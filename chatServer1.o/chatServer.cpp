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
    struct myMsg serverMsgs;
    int msqidServer=msgget(serverKey,0666);

    key_t chat1Key=ftok("chat1.cpp",'A');
    int msqidChat1=msgget(chat1Key,0666);

    key_t chat2Key=ftok("chat2.cpp",'A');
    int msqidChat2=msgget(chat2Key,0666);

    key_t chat3Key=ftok("chat3.cpp",'A');
    int msqidChat3=msgget(chat3Key,0666);
    while(1){
        msgrcv(msqidServer,&serverMsgs,sizeof(serverMsgs),0,0);
        cout<<serverMsgs.msgText<<" "<<serverMsgs.msgType<<endl;
        if(serverMsgs.msgType==1){
            msgsnd(msqidChat2,&serverMsgs,sizeof(serverMsgs),0);
            msgsnd(msqidChat3,&serverMsgs,sizeof(serverMsgs),0);
        }
        else if(serverMsgs.msgType==2){
            msgsnd(msqidChat1,&serverMsgs,sizeof(serverMsgs),0);
            msgsnd(msqidChat3,&serverMsgs,sizeof(serverMsgs),0);
        }
        else if(serverMsgs.msgType==3){
            msgsnd(msqidChat1,&serverMsgs,sizeof(serverMsgs),0);
            msgsnd(msqidChat2,&serverMsgs,sizeof(serverMsgs),0);
        }
    }
}


/*
// with fifos
int n=3;
    unordered_map<string,string> mp;
    while(n--){
        mkfifo("serverFifo",0666);
        int rfd=open("serverFifo",O_RDONLY);
        int hashFlag=0;
        string cpid,msg;
        char c;
        while(read(rfd,&c,sizeof(c))>0){
            if(hashFlag & c!='#') cpid.push_back(c);
            else if(c=='#'){
                if(hashFlag){
                    hashFlag=0;
                    mp[cpid]=msg;
                    cpid=""; msg="";
                }
                else hashFlag++;
            }
            else msg.push_back(c);
        }
        close(rfd);
    }
    for(auto each1:mp){
        for(auto each2:mp){
            sleep(1);
            if(each1.first==each2.first) continue;
            string name="fifo"+each2.first;
            mkfifo(name.c_str(),0666);
            int wfd=open(name.c_str(),O_WRONLY);
            write(wfd,each1.second.c_str(),each1.second.size());
            close(wfd);
        }
    }
    cout<<"Server Ends"<<endl;
*/