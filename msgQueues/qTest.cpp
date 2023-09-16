#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
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
    
    
    msgctl(msqidServer,IPC_RMID,NULL); // last argument can be NULL
    msgctl(msqidResponse,IPC_RMID,NULL); // last argument can be NULL
    // while(1){
    //     struct myMsg recieveMsg;
    //     msgrcv(msqidChat1,&recieveMsg,sizeof(recieveMsg),0,0);
    //     cout<<recieveMsg.msgType<<" 1 "<<recieveMsg.msgText<<endl;

    //     msgrcv(msqidChat2,&recieveMsg,sizeof(recieveMsg),0,0);
    //     cout<<recieveMsg.msgType<<" 2 "<<recieveMsg.msgText<<endl;

    //     msgrcv(msqidChat3,&recieveMsg,sizeof(recieveMsg),0,0);
    //     cout<<recieveMsg.msgType<<" 3 "<<recieveMsg.msgText<<endl;
    // }
}