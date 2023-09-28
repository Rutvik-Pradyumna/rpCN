#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
using namespace std;
struct myMsg{
    long type;
    char msgText[100];
    long fromPid;
};
int main(){
    key_t serverKey=ftok("chatClient.cpp",'Z');
    int msqidServer=msgget(serverKey,0666 | IPC_CREAT);

    set<long> st;
    while(1){
        myMsg receivedMsg;
        msgrcv(msqidServer,&receivedMsg,sizeof(receivedMsg),1,0);
        if(st.find(receivedMsg.fromPid)==st.end()){
            st.insert(receivedMsg.fromPid);
            cout<<"Added "<<receivedMsg.fromPid<<endl;
            continue;
        }
        long curPid=receivedMsg.fromPid;
        cout<<receivedMsg.msgText<<endl;
        for(auto each:st){
            if(curPid==each) continue;
            receivedMsg.type=each;
            msgsnd(msqidServer,&receivedMsg,sizeof(receivedMsg),0);
            kill((pid_t)each,SIGUSR1);
            cout<<(pid_t)curPid<<" "<<(pid_t)each<<endl;
        }
        sleep(3);
    }
}