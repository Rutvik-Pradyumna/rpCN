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
    sendMsg.msgType=3;
    int msqidServer=msgget(serverKey,0666 | IPC_CREAT);

    key_t chat3Key=ftok("chat3.cpp",'A');
    int msqidChat3=msgget(chat3Key,0666 | IPC_CREAT);
    struct myMsg recieveMsg;

    int n=2;
    while(n--){
        struct myMsg recieveMsg;
        msgrcv(msqidChat3,&recieveMsg,sizeof(recieveMsg),0,0);
        cout<<recieveMsg.msgText<<endl;
    }

    cin.getline(sendMsg.msgText,sizeof(sendMsg.msgText));
    int sent=msgsnd(msqidServer,&sendMsg,sizeof(sendMsg),0);
    if(sent==0) cout<<"Msg Sent"<<endl;
}