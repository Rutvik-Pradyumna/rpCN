#include<bits/stdc++.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/msg.h>
#include<sys/ipc.h>
using namespace std;
struct myMsg{
    long msgType;
    char msgText[500];
};
int main(){
    key_t serverKey = ftok("chatServer.cpp",'A');
    struct myMsg sendMsg;
    sendMsg.msgType=2;
    int msqidServer=msgget(serverKey,0666 | IPC_CREAT);

    key_t chat2Key=ftok("chat2.cpp",'A');
    int msqidChat2=msgget(chat2Key,0666 | IPC_CREAT);
    struct myMsg recieveMsg;

    msgrcv(msqidChat2,&recieveMsg,sizeof(recieveMsg),0,0);
    cout<<recieveMsg.msgText<<endl;

    cin.getline(sendMsg.msgText,sizeof(sendMsg.msgText));
    int sent=msgsnd(msqidServer,&sendMsg,sizeof(sendMsg),0);
    if(sent==0) cout<<"Msg Sent"<<endl;

    msgrcv(msqidChat2,&recieveMsg,sizeof(recieveMsg),0,0);
    cout<<recieveMsg.msgText<<endl;
}