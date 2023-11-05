#include<bits/stdc++.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;
#define PATH "tos2"
#define PATH2 "tos1"

int getFD(){
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);

    struct sockaddr_un myAddr;
    myAddr.sun_family=AF_UNIX;
    strcpy(myAddr.sun_path,PATH);
    unlink(PATH);
    if(bind(usfd,(struct sockaddr*)&myAddr,sizeof(myAddr))<0){
        perror("bind");
        exit(EXIT_FAILURE);
    }
    char msg[10];
    struct iovec ioVector;
    ioVector.iov_base=msg;     ioVector.iov_len=sizeof(msg);

    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr socket_msg;
    socket_msg.msg_name=NULL;           socket_msg.msg_namelen=0;
    socket_msg.msg_iov=&ioVector;       socket_msg.msg_iovlen=1;
    socket_msg.msg_control=cmsg;        socket_msg.msg_controllen=sizeof(cmsg);
    socket_msg.msg_flags=0;

    cout<<"Waiting to recv fd..."<<endl;
    if(recvmsg(usfd,&socket_msg,0)<0){
        perror("recvmsg");
        exit(EXIT_FAILURE);
    }
    cout<<"got "<<msg<<endl;
    struct cmsghdr *c=CMSG_FIRSTHDR(&socket_msg);
    return *(int*)CMSG_DATA(c);
}

int main(){
    int nsfd=getFD();
    if(nsfd>0) cout<<"Recieved nsfd"<<endl;

    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
    struct sockaddr_un s1Addr;
    s1Addr.sun_family=AF_UNIX;
    strcpy(s1Addr.sun_path,PATH2);

    while(1){
        char msg[20]="From s2";
        send(nsfd,msg,sizeof(msg),0);
        char buff[20]={'\0'};
        recv(nsfd,buff,sizeof(buff),0);
        int st=sendto(usfd,buff,sizeof(buff),0,(struct sockaddr*)&s1Addr,sizeof(s1Addr));
        cout<<"got "<<buff<<" "<<st<<endl;
    }

}