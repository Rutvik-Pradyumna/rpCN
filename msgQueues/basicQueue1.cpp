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
    key_t key=ftok("basicQueue1.cpp",'A');
    struct myMsg msg;
    int msqid=msgget(key,0666 | IPC_CREAT);
    msg.msgType=1;

    cout<<"Type msg : ";
    cin.getline(msg.msgText,sizeof(msg.msgText));
    
    int sent=msgsnd(msqid,&msg,sizeof(msg),0);
    if(sent==0) cout<<"Msg Sent"<<endl;
    // struct msqid_ds queueInfo;
    // msgctl(msqid,IPC_RMID,&queueInfo); // last argument can be NULL
}