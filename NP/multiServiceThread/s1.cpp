#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;
struct sockaddr_in sAddr,cAddr;
int adrlen=sizeof(sAddr);
struct ThreadArgs {
    int nsfd;
};

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

void* server(void *args){
    cout<<"In Thread"<<endl;
    ThreadArgs *myArgs=(ThreadArgs* )args;
    while(1){
        string s="From Server"+to_string(myArgs->nsfd);
        send(myArgs->nsfd,s.c_str(),sizeof(s),0);
        sleep(5);
    }
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
    while(1){
        int nsfd=accept(sfd,(struct sockaddr*)&cAddr,(socklen_t *)&adrlen);
        if(nsfd<0){
            perror("err in server acc");
            continue;
        }
        char c;
        recv(nsfd,&c,sizeof(c),0);
        pthread_t newTh;
        ThreadArgs args;
        args.nsfd=nsfd;
        pthread_create(&newTh,NULL,server,&args);
    }
    return 0;
}