#include<bits/stdc++.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/msg.h>
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

    key_t chat1Key=ftok("chat1.cpp",'A');
    int msqidChat1=msgget(chat1Key,0666 | IPC_CREAT);

    cin.getline(sendMsg.msgText,sizeof(sendMsg.msgText));
    int sent=msgsnd(msqidServer,&sendMsg,sizeof(sendMsg),0);
    if(sent==0) cout<<"Msg Sent"<<endl;

    int n=2;
    while(n--){
        struct myMsg recieveMsg;
        msgrcv(msqidChat1,&recieveMsg,sizeof(recieveMsg),0,0);
        cout<<recieveMsg.msgText<<endl;
    }
}

/*
// with fifos
int cpid=getpid();
    mkfifo("serverFifo",0666);
    string s;
    cout<<"s is : ";
    getline(cin,s);
    int wfd=open("serverFifo",O_WRONLY);
    s+='#'+to_string(cpid)+'#';
    write(wfd,s.c_str(),s.size());
    close(wfd);
    cout<<"chat "<<cpid<<endl;
    string name="fifo"+to_string(cpid);
    mkfifo(name.c_str(),0666);
    int n=2;
    while(n--){
        int rfd=open(name.c_str(),O_RDONLY);
        char c;
        while(read(rfd,&c,sizeof(c))>0) cout<<c;
        close(rfd);
    }
    cout<<"\nEnd\n";
*/