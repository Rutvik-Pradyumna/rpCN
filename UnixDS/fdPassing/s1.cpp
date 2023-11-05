#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/un.h>
using namespace std;
#define PATH "tos2"
#define PATH2 "tos1"

struct sockaddr_in sAddr,cAddr;
int adrlen=sizeof(sAddr);
/*
Notes -
1) sendmsg and recvmsg needs msghdr to describe data sent
2) msghdr uses iovec & cmsghdr
3) cmsghdr actually defines the header of the data, which should be 
followed by an unsigned char array that holds the actual data of the control information
4) 
*/
void createSocket(int &sfd){
    if((sfd=socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket err");
        exit(1);
    }
}

void setSockOpt(int &sfd){
    int opt=1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0){
        perror("sockopt err");
        exit(1);
    }
}

void bindSocket(int &sfd,int port){
    sAddr.sin_family=AF_INET;
    sAddr.sin_addr.s_addr=INADDR_ANY;
    sAddr.sin_port=htons(port);
    if(bind(sfd, (struct sockaddr*)&sAddr, adrlen)<0){
        perror("bind err");
        exit(1);
    }
}

void listenSocket(int &sfd){
    if(listen(sfd,3)<0){
        perror("listen err");
        exit(1);
    }
}

void sendFD(int &fd){
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);

    struct sockaddr_un myAddr;
    myAddr.sun_family=AF_UNIX;
    strcpy(myAddr.sun_path,PATH);

    struct iovec ioVector;
    char msg[8]="From s1";
    ioVector.iov_base=msg;     ioVector.iov_len=sizeof(msg);

    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr socket_msg;
    socket_msg.msg_name=(void*)&myAddr;     socket_msg.msg_namelen=sizeof(myAddr);
    socket_msg.msg_iov=&ioVector;           socket_msg.msg_iovlen=1;
    socket_msg.msg_control=cmsg;            socket_msg.msg_controllen=sizeof(cmsg);
    socket_msg.msg_flags=0;

    struct cmsghdr *c = CMSG_FIRSTHDR(&socket_msg);
    c->cmsg_level=SOL_SOCKET;   c->cmsg_type=SCM_RIGHTS;
    c->cmsg_len=CMSG_LEN(sizeof(int));
    *(int*)CMSG_DATA(c) = fd;
    if(sendmsg(usfd, &socket_msg, 0)<0){
        perror("sendmsg err");
        exit(1);
    }
    cout<<"sent sfd"<<endl;
}

int main(){
    int sfd;
    createSocket(sfd);
    setSockOpt(sfd);
    int port;
    cout<<"Enter port : ";
    cin>>port;
    bindSocket(sfd,port);
    listenSocket(sfd);
    int nsfd;
    if((nsfd=accept(sfd,(struct sockaddr*)&cAddr,(socklen_t *)&adrlen))<0){
        perror("err in server acc");
        exit(1);
    }
    cout<<"Accepted"<<endl;

    char msg[20]="From s1";
    send(nsfd,msg,sizeof(msg),0);
    char buff[20]={'\0'};
    recv(nsfd,buff,sizeof(buff),0);
    cout<<"got "<<buff<<endl;

    sendFD(nsfd);
    
    struct sockaddr_un s1Addr;
    s1Addr.sun_family=AF_UNIX;
    strcpy(s1Addr.sun_path,PATH2);
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
    unlink(PATH2);
    if(bind(usfd,(struct sockaddr*)&s1Addr,sizeof(s1Addr))<0){
        perror("usfd bind");
        exit(EXIT_FAILURE);
    }
    
    while(1){
        char msg[20]={'\0'};
        recvfrom(usfd,msg,sizeof(msg),0,(struct sockaddr*)&s1Addr,(socklen_t*)&adrlen);
        cout<<"got "<<msg<<endl;
    }
}