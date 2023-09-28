#include<bits/stdc++.h>
#include<unistd.h>
#include<csignal>
using namespace std;
int main(){
    cout<<"pid "<<getpid()<<endl;
    cout<<"ppid "<<getppid()<<endl;
    cout<<"pgrp "<<getpgrp()<<endl;
    cout<<"sid "<<getsid(0)<<endl;
    int n;
    cin>>n;
    if(n!=-1){
        setpgid(0,n);
        cout<<"npgrp "<<getpgrp()<<endl;
        cout<<"nsid "<<getsid(0)<<endl;
    }
    while(1);
    return 0;
}