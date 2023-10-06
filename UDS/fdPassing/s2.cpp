#include<bits/stdc++.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;
#define PATH "fdShare"

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

    struct iovec ioVector;
    ioVector.iov_base=NULL;     ioVector.iov_len=0;

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

    struct cmsghdr *c=CMSG_FIRSTHDR(&socket_msg);
    return *(int*)CMSG_DATA(c);
}

int main(){
    int nsfd=getFD();
    if(nsfd>0) cout<<"Recieved nsfd"<<endl;

    while(1){
        char msg[20]="From s2";
        send(nsfd,msg,sizeof(msg),0);
        char buff[20]={'\0'};
        recv(nsfd,buff,sizeof(buff),0);
        cout<<"got "<<buff<<endl;
    }

}