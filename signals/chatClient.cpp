#include<bits/stdc++.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<poll.h>
#include<signal.h>
using namespace std;
struct myMsg{
    long type;
    char msgText[100];
    long fromPid;
};
long msgType=(long)getpid();
int msqid;

void handler(int sigNum){
    struct myMsg recvMsg;
    cout<<"inh"<<endl;
    msgrcv(msqid,&recvMsg,sizeof(recvMsg),msgType,0);
    cout<<"From : "<<recvMsg.fromPid<<" "<<recvMsg.msgText<<endl;
    cout<<"inh2"<<endl;
    // raise(SIGTERM);
    // sleep(3);
}

void sender(int sigNum) {
    struct myMsg snd;
    snd.type=1;
    snd.fromPid=msgType;
    cout<<"Enter Msg "<<endl;
    read(0,snd.msgText,sizeof(snd.msgText));
    snd.msgText[sizeof(snd.msgText) - 1] = '\0';
    msgsnd(msqid,&snd,sizeof(snd),0);
    sender(0);
}

int main(){
    signal(SIGUSR1,handler);
    // signal(SIGTERM,sender);
    key_t serverKey = ftok("chatClient.cpp",'Z');
    msqid=msgget(serverKey,0666 | IPC_CREAT);
    struct myMsg sendMsg;
    sendMsg.type=1;
    sendMsg.fromPid=msgType;
    cout<<msgType<<endl;
    msgsnd(msqid,&sendMsg,sizeof(sendMsg),0);
    
    // sender(0);

    while(1){
        struct myMsg snd;
        snd.type=1;
        snd.fromPid=msgType;
        cout<<"Enter Msg "<<endl;
        read(0,snd.msgText,sizeof(snd.msgText));
        snd.msgText[sizeof(snd.msgText) - 1] = '\0';
        msgsnd(msqid,&snd,sizeof(snd),0);
    }

}
