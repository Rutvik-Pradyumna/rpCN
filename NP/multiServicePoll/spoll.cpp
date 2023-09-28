#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<poll.h>
#include<arpa/inet.h>
#include<sys/shm.h>
using namespace std;
struct sockaddr_in sAddr,c1Addr,c2Addr;
int adrlen=sizeof(sAddr);
struct shMemory{
    bool s1;
    bool s2;
};

void createSockets(int sfd[]){
    if((sfd[0]=socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket1 err");
        exit(1);
    }
    if((sfd[1]=socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("socket2 err");
        exit(1);
    }
}

void setSockOpts(int sfd[]){
    int opt=1;
    if(setsockopt(sfd[0], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0){
        perror("sockopt1 err");
        exit(1);
    }
    if(setsockopt(sfd[1], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0){
        perror("sockopt2 err");
        exit(1);
    }
}

void bindSockets(int sfd[]){
    sAddr.sin_family=AF_INET;
    sAddr.sin_addr.s_addr=INADDR_LOOPBACK;
    sAddr.sin_port=htons(9000);
    if(bind(sfd[0], (struct sockaddr*)&sAddr, adrlen)<0){
        perror("bind1 err");
        exit(1);
    }
    sAddr.sin_port=htons(9001);
    if(bind(sfd[1], (struct sockaddr*)&sAddr, adrlen)<0){
        perror("bind2 err");
        exit(1);
    }
}

void listenSockets(int sfd[]){
    if(listen(sfd[0],3)<0){
        perror("listen1 err");
        exit(1);
    }
    if(listen(sfd[1],3)<0){
        perror("listen2 err");
        exit(1);
    }
}

int main(){
    int sfd[2]={0};
    createSockets(sfd);
    setSockOpts(sfd);
    bindSockets(sfd);
    listenSockets(sfd);
    int nsfd1,nsfd2;
    if((nsfd1 = accept(sfd[0],(struct sockaddr*)&c1Addr,(socklen_t*)&adrlen))<0){
        perror("sfd1 err");
        exit(1);
    }
    cout<<"S1 connected"<<endl;
    if((nsfd2 = accept(sfd[1],(struct sockaddr*)&c2Addr,(socklen_t*)&adrlen))<0){
        perror("sfd2 err");
        exit(1);
    }
    cout<<"S2 connected"<<endl;
    struct pollfd psfd[2];
    memset(&psfd,0,sizeof(psfd));
    psfd[0].fd=nsfd1;  psfd[0].events=POLLIN;
    psfd[1].fd=nsfd2;  psfd[1].events=POLLIN;

    // shm for s1 s2 availability
    key_t key=ftok("spoll.cpp",'A');
    int shmid=shmget(key,sizeof(shMemory),0666 | IPC_CREAT);
    shMemory *shmPtr=(struct shMemory*)shmat(shmid,NULL,0);
    shmPtr->s1=1;
    shmPtr->s2=1;

    while(1){
        poll(psfd,2,-1);
        int c=0;
        if(shmPtr->s1 && (psfd[0].revents & POLLIN)){
            c=fork();
            if(c==0){
                shmPtr->s1=0;
                dup2(nsfd1,2);
                execlp("./s1poll","s1poll",NULL);
            }
            else while(shmPtr->s1);
        }
        if(shmPtr->s2 && (psfd[1].revents & POLLIN)){
            c=fork();
            if(c==0){
                shmPtr->s2=0;
                dup2(nsfd2,2);
                execlp("./s2poll","s2poll",NULL);
            }
            else while(shmPtr->s2);
        }
    }
}