#include<bits/stdc++.h>
#include<csignal>
using namespace std;


// Single Handler
int c=0;
void myHandler(int sigNo){
    if(c>0){
        cout<<"In Parent Handler"<<endl;
        signal(SIGINT,SIG_DFL);
    }
    else{
        cout<<"In Child Handler "<<endl;                
        kill(getppid(),SIGINT);
    }
}
int main(){
    c=fork();
    if(c>0){
        signal(SIGINT,myHandler);
        sleep(2);
        kill(c,SIGINT);
        while(1);
    }
    else{
        signal(SIGINT,myHandler);
        while(1);
    }
}

/*
// Double Handler
void parentHandler(int sigNo){
    cout<<"In Parent Handler"<<endl;
    signal(SIGINT,SIG_DFL);
}
void childHandler(int sigNo){
    cout<<"In Child Handler"<<endl;
    kill(getppid(),SIGINT);
    signal(SIGINT,SIG_DFL);
}
int main(){
    int c=fork();
    if(c>0){
        signal(SIGINT,parentHandler);
        sleep(2);
        kill(c,SIGINT);
        while(1);
    }
    else{
        signal(SIGINT,childHandler);
        while(1);
    }
}
*/