#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
using namespace std;
struct Msg{
    long type;
    char text[500];
    long frmPid;
};

long pid=(long)getpid();
key_t serverKey=ftok("chatServer.cpp",'A');
int msqidServer=msgget(serverKey,0666 | IPC_CREAT);
key_t clientKey=ftok("chatClient.cpp",'A');
int msqidClient=msgget(clientKey,0666 | IPC_CREAT);

void* sendMsg(void* args){
    while(1){
        Msg sender;
        cin.getline(sender.text,sizeof(sender.text));
        sender.type=pid;
        msgsnd(msqidServer,&sender,sizeof(sender),0);
        cout<<"You : "<<sender.text<<endl;
    }
}
void* receiveMsg(void* args){
    while(1){
        Msg receiver;
        msgrcv(msqidClient,&receiver,sizeof(receiver),pid,0);
        cout<<receiver.frmPid<<" : "<<receiver.text<<endl;
    }
}

int main(){
    Msg registerClient;
    registerClient.type=1;
    strcpy(registerClient.text, to_string(pid).c_str());
    cout<<registerClient.text<<endl;
    msgsnd(msqidServer,&registerClient,sizeof(registerClient),0);

    pthread_t sendThread,receiveThread;
    pthread_create(&sendThread,NULL,sendMsg,NULL);
    pthread_create(&receiveThread,NULL,receiveMsg,NULL);
    pthread_join(sendThread,NULL);
    pthread_join(receiveThread,NULL);
}