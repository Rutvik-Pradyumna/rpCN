#include<bits/stdc++.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;

void createSocket(int &sfd){
    if((sfd=socket(AF_UNIX, SOCK_STREAM, 0))<0){
        perror("udp socket err");
        exit(1);
    }
}

void bindSocket(int &sfd,char path[]){
    struct sockaddr_un addr;
    addr.sun_family=AF_UNIX;
    unlink(path);
    strcpy(addr.sun_path,path);
    if(bind(sfd, (struct sockaddr*)&addr, sizeof(addr))<0){
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

void sendFD(int usfd,int fd){
    struct sockaddr_un addr;
    int adrlen=sizeof(addr);
    getsockname(usfd, (struct sockaddr*)&addr, (socklen_t*)&adrlen);
    cout << "addr in send_fd " << addr.sun_path << endl;

    struct iovec ioVector;
    char msg[8]="From s1";
    ioVector.iov_base=msg;     ioVector.iov_len=sizeof(msg);

    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr socket_msg;
    socket_msg.msg_name=NULL;     socket_msg.msg_namelen=0;
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
    cout<<"sent fd"<<endl;
}

int main(){
    int usfd;
    createSocket(usfd);
    char path[20];
    cout<<"Enter path : ";
    cin>>path;
    bindSocket(usfd,path);
    listenSocket(usfd);
    int nsfd;
    if((nsfd=accept(usfd,NULL,NULL))<0) perror("accept");
    cout<<"Accepted"<<endl;
    sleep(2);
    int pp[2];
    pipe(pp);
    char msg[10]="from s1";
    write(pp[1],msg,sizeof(msg));
    sendFD(nsfd,pp[0]);
    while(1);
}