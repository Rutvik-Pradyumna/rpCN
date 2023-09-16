#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
using namespace std;
struct sharedMemory{
    char text[500];
    bool p;
};
int main(){
    key_t shmKey=ftok("shmP1.cpp",'A');
    int shmid=shmget(shmKey,sizeof(sharedMemory),0666 | IPC_CREAT);
    cout<<"key "<<shmKey<<" id "<<shmid<<endl;

    sharedMemory *shmPtr=(struct sharedMemory*)shmat(shmid,NULL,0);
    cout<<shmPtr->text<<" "<<shmPtr->p<<endl;

    shmdt((void*)shmPtr);

    cout<<"\nP2 ends"<<endl;
}