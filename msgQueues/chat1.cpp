#include<bits/stdc++.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<poll.h>
using namespace std;
struct myMsg{
    long msgType;
    char msgText[500];
};
int main(){
    key_t serverKey = ftok("chatServer.cpp",'A');
    struct myMsg sendMsg;
    sendMsg.msgType=1;
    int msqidServer=msgget(serverKey,0666 | IPC_CREAT);

    key_t responseKey=ftok("chatServer.cpp",'B');
    myMsg responseMsgs;
    int msqidResponse=msgget(responseKey,0666 | IPC_CREAT);

    struct pollfd pfds[1];
    pfds[0].fd=0; pfds[0].events=POLLIN;
    int reply;
    while(1){
        int ret=poll(pfds,1,10);
        if(ret>0 && (pfds[0].revents & POLLIN)){
            int bytesRead=read(0,sendMsg.msgText,sizeof(sendMsg.msgText));
            sendMsg.msgText[bytesRead]='\0';
            msgsnd(msqidServer,&sendMsg,sizeof(sendMsg),0);
        }
        reply=msgrcv(msqidResponse,&responseMsgs,sizeof(responseMsgs),4,IPC_NOWAIT);
        if(reply!=-1) cout<<"2 : "<<responseMsgs.msgText<<endl;
        reply=msgrcv(msqidResponse,&responseMsgs,sizeof(responseMsgs),5,IPC_NOWAIT);
        if(reply!=-1) cout<<"3 : "<<responseMsgs.msgText<<endl;
    }

}
