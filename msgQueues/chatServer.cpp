#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/msg.h>
#include<sys/ipc.h>
using namespace std;
struct myMsg{
    long msgType;
    char msgText[500];
};
int main(){
    key_t serverKey=ftok("chatServer.cpp",'A');
    myMsg serverMsgs;
    int msqidServer=msgget(serverKey,0666 | IPC_CREAT);

    key_t responseKey=ftok("chatServer.cpp",'B');
    myMsg responseMsgs;
    int msqidResponse=msgget(responseKey,0666 | IPC_CREAT);
    int reply;
    while(1){
        reply=msgrcv(msqidServer,&responseMsgs,sizeof(responseMsgs),0,IPC_NOWAIT);
        if(reply!=-1){
            if(responseMsgs.msgType==1){
                responseMsgs.msgType=6;
                msgsnd(msqidResponse,&responseMsgs,sizeof(responseMsgs),0);
                responseMsgs.msgType=8;
                msgsnd(msqidResponse,&responseMsgs,sizeof(responseMsgs),0);
            }
            else if(responseMsgs.msgType==2){
                responseMsgs.msgType=4;
                msgsnd(msqidResponse,&responseMsgs,sizeof(responseMsgs),0);
                responseMsgs.msgType=9;
                msgsnd(msqidResponse,&responseMsgs,sizeof(responseMsgs),0);
            }
            else if(responseMsgs.msgType==3){
                responseMsgs.msgType=5;
                msgsnd(msqidResponse,&responseMsgs,sizeof(responseMsgs),0);
                responseMsgs.msgType=7;
                msgsnd(msqidResponse,&responseMsgs,sizeof(responseMsgs),0);
            }
        }
    }
}