#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;
struct sockaddr_in sAddr,cAddr;
int adrlen=sizeof(sAddr);

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
    while(1){
        char buff[500];
        int sz;
        if((sz=recv(nsfd,buff,sizeof(buff),0))<0){
            perror("err in server recv");
            exit(1);
        }
        buff[sz]='\0';
        cout<<"From port-"<<port<<" "<<buff<<endl;
    }
}

void* joinThreads(void *args){
    while(1){
        cout<<"Enter 1 to join new thread 0 to close"<<endl;
        int in;
        cin>>in;
        if(in==0) return NULL;
        else if(in==1){
            pthread_t newTh;
            pthread_create(&newTh,NULL,server,NULL);
            sleep(5);
        }
    }
}

int main(){
    pthread_t th;
    pthread_create(&th,NULL,joinThreads,NULL);
    pthread_join(th,NULL);
    return 0;
}