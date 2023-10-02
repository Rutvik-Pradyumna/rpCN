#include<bits/stdc++.h>
using namespace std;
int main(){
    int pid;
    cout<<"Enter pid ";
    cin>>pid;
    while(1){
        int k;
        cout<<"Enter 1 to send signal ";
        cin>>k;
        if(k==1) kill(pid,SIGUSR1);
    }
}