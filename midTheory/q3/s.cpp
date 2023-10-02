#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;

struct sockaddr_in cAddr;
int adrlen=sizeof(cAddr);
struct ThreadArgs {
    int nsfd;
};

void* serve(void *args){
    ThreadArgs *myArgs=(ThreadArgs* )args;
    string msg="From "+to_string(getpid());
    int curNsfd=dup(myArgs->nsfd);
    while(1){
        send(curNsfd,msg.c_str(),msg.size(),0);
        sleep(3);
    }
}

void handler(int sig){
    int nsfd;
    if((nsfd=accept(2,(struct sockaddr*)&cAddr,(socklen_t*)&adrlen))<0){
        perror("accept err");
    }
    ThreadArgs nsfdArg;
    nsfdArg.nsfd=nsfd;
    pthread_t th;
    pthread_create(&th,NULL,serve,&nsfdArg);
}

int main(){
    signal(SIGUSR1,handler);
    cout<<"started s of pid : "<<getpid()<<endl;
    kill(getppid(),SIGUSR1);
    while(1);
}