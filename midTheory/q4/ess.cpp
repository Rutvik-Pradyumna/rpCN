#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;
/*
ref for sharing fd's
https://www.sobyte.net/post/2022-01/pass-fd-over-domain-socket/
*/
struct sockaddr_in addr,cAddr;
int adrlen=sizeof(addr);

void createSocket(int &sfd){
    if((sfd=socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("tcp socket err");
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
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port=htons(port);
    if(bind(sfd, (struct sockaddr*)&addr, adrlen)<0){
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

int main(){
    int sfd;
    createSocket(sfd);
    setSockOpt(sfd);
    bindSocket(sfd,8080);
    listenSocket(sfd);
    cout<<"Waiting to accept"<<endl;
    int nsfd=accept(sfd,(struct sockaddr*)&cAddr,(socklen_t*)&adrlen);
    if(nsfd<0){
        perror("accept err");
        exit(0);
    }
    cout<<"Accepted"<<endl;
    char buff[11]="Server msg";
    while(1){
        send(nsfd,buff,sizeof(buff),0);
        char msg[50]={'\0'};
        recv(nsfd,msg,sizeof(msg),0);
        cout<<msg<<endl;
        sleep(1);
    }
}