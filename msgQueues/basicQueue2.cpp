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
    int msqid=msgget(key,0666);

    msgrcv(msqid,&msg,sizeof(msg),1,0);
    cout<<msg.msgText<<endl;
    
}
