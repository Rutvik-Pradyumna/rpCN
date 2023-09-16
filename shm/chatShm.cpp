#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/msg.h>
using namespace std;
struct shMemory{
    long count;
};
struct msgBuff{
    long type;
    char text[500];
    long from;
};
msgBuff myMsg;
shMemory *shmPtr;
int msqid;

void* sendMsg(void *args){
    while(1){
        msgBuff sendBuff;
        cin.getline(sendBuff.text,sizeof(sendBuff.text));
        cout<<"You : "<<sendBuff.text<<endl;
        for(int i=1;i<shmPtr->count+1;i++){
            if(i==myMsg.type) continue;
            sendBuff.type=i;
            sendBuff.from=myMsg.type;
            msgsnd(msqid,&sendBuff,sizeof(sendBuff),0);
        }
    }
}

void* receiveMsg(void *args){
    while(1){
        msgBuff receiveBuff;
        msgrcv(msqid,&receiveBuff,sizeof(receiveBuff),myMsg.type,0);
        cout<<receiveBuff.from<<" : "<<receiveBuff.text<<endl;
    }
}

int main(){
    key_t key=ftok("chatShm.cpp",'A');
    int shmid=shmget(key,sizeof(shMemory),0666 | IPC_CREAT);
    cout<<"key "<<key<<" id "<<shmid<<endl;
    msqid=msgget(key,0666 | IPC_CREAT);
    // shmctl(shmid, IPC_RMID, NULL);
    // msgctl(msqid,IPC_RMID,NULL);

    shmPtr=(struct shMemory*)shmat(shmid,NULL,0);
    if(shmPtr->count==94154057335488) shmPtr->count=1;
    else shmPtr->count++;
    myMsg.type=shmPtr->count;
    cout<<shmPtr->count<<endl;

    pthread_t sendThread,receiveThread;
    pthread_create(&sendThread,NULL,sendMsg,NULL);
    pthread_create(&receiveThread,NULL,receiveMsg,NULL);
    pthread_join(sendThread,NULL);
    pthread_join(receiveThread,NULL);
}