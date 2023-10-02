#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<poll.h>
using namespace std;

struct sockaddr_in sAddr,cAddr;
int adrlen=sizeof(sAddr);
int nsfdArr[100];
int startInd=0,endInd=0;

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


void handler(int sigNum){
    if(sigNum!=SIGUSR1) return;
    cout<<"Recieved Signal From P4"<<endl;
    int c=fork();
    if(c>0){
        startInd++;
    }
    else{
        dup2(nsfdArr[startInd],2);
        execlp("./echo","echo",NULL);
    }
}

int main(){
    // opening sfd
    signal(SIGUSR1,handler);
    cout<<"pid "<<getpid()<<endl;
    int sfd;
    createSocket(sfd);
    setSockOpt(sfd);
    int port;
    cout<<"Enter port : ";
    cin>>port;
    bindSocket(sfd,port);
    listenSocket(sfd);

    int pp[2];
    pipe(pp);
    int c=fork();
    if(c>0){
        close(pp[1]);
        sleep(1);
        struct pollfd pfd[5];
        pfd[0].fd=sfd;  pfd[1].fd=0;
        pfd[2].fd=fileno(popen("./p3","r"));
        mkfifo("fifo",0666);
        pfd[3].fd=open("fifo",O_RDONLY); // must open writind fd also for non blocking
        pfd[4].fd=pp[0];
        for(int i=0;i<5;i++) pfd[i].events=POLLIN;
        while(1){
            poll(pfd,5,-1);
            if(pfd[0].revents & POLLIN){
                if((nsfdArr[endInd]=accept(sfd,(struct sockaddr*)&cAddr,(socklen_t*)&adrlen))<0){
                    perror("err in accept");
                    exit(1);
                }
                endInd++;
            }
            else{
                for(int i=1;i<5;i++){
                    if(pfd[i].revents & POLLIN){
                        char buff[100]={'\0'};
                        read(pfd[i].fd,buff,sizeof(buff));
                        for(int j=startInd;j<endInd;j++){
                            send(nsfdArr[j],buff,sizeof(buff),0);
                        }
                    }
                }
            }
        }
    }
    else{
        close(pp[0]);
        dup2(pp[1],1);
        execlp("./p1","p1",NULL);
    }
}