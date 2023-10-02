#include<bits/stdc++.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/msg.h>
#include<csignal>
using namespace std;

int sfd=-1,msqid;
struct msg{
    long type;
    char pid[20];
};

int getSfd(){
    cout<<"My pid : "<<getpid()<<endl;
    int usfd=socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un un;
    un.sun_family = AF_UNIX;
    unlink("process_a");
    strcpy(un.sun_path, "process_a");

    if (bind(usfd, (struct sockaddr*)&un, sizeof(un)) < 0) {
        perror("usfd bind err");
        return 1;
    }
    char buf[512];
    struct iovec e = {buf, 512};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr m = {NULL, 0, &e, 1, cmsg, sizeof(cmsg), 0};
    if(recvmsg(usfd, &m, 0)<0){
        perror("usfd recvmsg err");
        exit(1);
    }
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    int sfd = *(int*)CMSG_DATA(c); // receive file descriptor
    return sfd;
}

void sendSfd(int sig){
    sleep(1);
    int usfd=socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un uAddr;
    uAddr.sun_family = AF_UNIX;
    strcpy(uAddr.sun_path, "process_a");

    struct iovec e = {NULL, 0};
    char cmsg[CMSG_SPACE(sizeof(int))]; // allocating space for control data
    struct msghdr m = {(void*)&uAddr, sizeof(uAddr), &e, 1, cmsg, sizeof(cmsg), 0};
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    c->cmsg_level = SOL_SOCKET;
    c->cmsg_type = SCM_RIGHTS;
    c->cmsg_len = CMSG_LEN(sizeof(int));
    *(int*)CMSG_DATA(c) = sfd;
    if(sendmsg(usfd, &m, 0)<0){
        perror("sendmsg err");
        exit(1);
    }
    cout<<"sent sfd"<<endl;
}

void handler(int sig,siginfo_t* info,void* context){
    if(sfd==-1){
        kill(info->si_pid,SIGUSR2);
        sfd=getSfd();
    }
    int k=3;
    while(k--){
        char recvMsg[100]={'\0'};
        recv(sfd,recvMsg,sizeof(recvMsg),0);
        cout<<recvMsg<<endl;
        char sendMsg[8]="From C1";
        send(sfd,sendMsg,sizeof(sendMsg),0);
    }
    msg myMsg;
    myMsg.type=1;
    strcpy(myMsg.pid,to_string(getpid()).c_str());
    msgsnd(msqid,&myMsg,sizeof(myMsg),0);
    msgrcv(msqid,&myMsg,sizeof(myMsg),0,0);
    string pid="";
    int sz=strlen(myMsg.pid);
    for(int i=0;i<sz;i++) pid += myMsg.pid[i];
    kill(stoi(pid),SIGUSR1);
}

int main(){
    cout<<"My pid : "<<getpid()<<endl;
    msqid=msgget(10,0666 | IPC_CREAT);
    msg myMsg;
    myMsg.type=1;
    strcpy(myMsg.pid,to_string(getpid()).c_str());
    msgsnd(msqid,&myMsg,sizeof(myMsg),0);

    struct sigaction sa;
    sa.sa_flags=SA_SIGINFO;
    sa.sa_sigaction=handler;
    sigaction(SIGUSR1,&sa,NULL);
    signal(SIGUSR2,sendSfd);
    
    while(1);
}