#include<bits/stdc++.h>
#include<unistd.h>
#include<csignal>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std; 

struct shMemory{
    int val;
};
shMemory *shmPtr1,*shmPtr2;

void myHandler(int sigNo,siginfo_t *info,void *context){
    cout<<"In Handler "<<shmPtr1->val<<endl;
    shmPtr1->val++;
    if(shmPtr1->val==5) return;
    kill(info->si_pid,SIGUSR1);
}
int main(){
    cout<<getpid()<<endl;

    // initialising sigaction
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags=SA_SIGINFO;
    sa.sa_sigaction=myHandler;
    sigaction(SIGUSR1,&sa,NULL);

    // initialising shm-1
    key_t key1=ftok("shmP1.cpp",'A');
    int shm1id=shmget(key1,sizeof(shMemory),0666 | IPC_CREAT);
    shmPtr1=(struct shMemory*)shmat(shm1id,NULL,0);
    shmPtr1->val=0;

    // initialising shm-2
    key_t key2=ftok("shmP2.cpp",'A');
    int shm2id=shmget(key2,sizeof(shMemory),0666 | IPC_CREAT);
    shmPtr2=(struct shMemory*)shmat(shm2id,NULL,0);
    shmPtr2->val=0;

    mkfifo("sigactFifo",0666);
    int rfd=open("sigactFifo",O_RDONLY);
    char c;
    string pid;
    while(read(rfd,&c,sizeof(c))>0) pid.push_back(c);
    close(rfd);
    kill(stoi(pid),SIGUSR1);

    while(1);
}