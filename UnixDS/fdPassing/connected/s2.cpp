#include<bits/stdc++.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;

void createSocket(int &sfd){
    if((sfd=socket(AF_UNIX, SOCK_STREAM, 0))<0){
        perror("socket err");
        exit(1);
    }
}

int getFD(int usfd){
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
    int usfd;
    createSocket(usfd);
    char path[20]="mypath";
    cout<<"Enter path : ";
    cin>>path;
    struct sockaddr_un addr;
    addr.sun_family=AF_UNIX;
    strcpy(addr.sun_path,path);
    if(connect(usfd,(struct sockaddr*)&addr,sizeof(addr))<0) perror("connect");

    int fd=getFD(usfd);
    char msg[20];
    read(fd,msg,sizeof(msg));
    cout<<"msg "<<msg<<endl;
    while(1);
}