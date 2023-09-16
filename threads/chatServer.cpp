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
int main(){
    key_t serverKey=ftok("chatServer.cpp",'A');
    int msqidServer=msgget(serverKey,0666 | IPC_CREAT);

    key_t clientKey=ftok("chatClient.cpp",'A');
    int msqidClient=msgget(clientKey,0666 | IPC_CREAT);

    set<string> st;
    while(1){
        Msg receivedMsg;
        msgrcv(msqidServer,&receivedMsg,sizeof(receivedMsg),0,0);
        if(receivedMsg.type==1){
            st.insert(receivedMsg.text);
        }
        else{
            string pid=to_string(receivedMsg.type);
            receivedMsg.frmPid=receivedMsg.type;  
            cout<<receivedMsg.text<<endl;
            for(auto each:st){
                if(pid==each) continue;
                receivedMsg.type=stol(each);
                msgsnd(msqidClient,&receivedMsg,sizeof(receivedMsg),0);
                cout<<pid<<" "<<each<<endl;
            }
        }
    }
}