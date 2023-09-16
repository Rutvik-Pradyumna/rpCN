#include<bits/stdc++.h>
#include<signal.h>       // to use signal system calls
using namespace std;

int c=0;
void myHandler(int sigNo){
    cout<<"In Handler Function "<<sigNo<<endl;
    c++;
    if(c==5) signal(SIGINT,SIG_DFL);
}

int main(){
    // using signal()
    signal(SIGINT,myHandler);
    while(1);
    cout<<"In Main"<<endl;
}

/*
// using raise()
    signal(SIGUSR1,myHandler);
    raise(SIGUSR1);
*/