#include<bits/stdc++.h>
#include<sys/socket.h>
using namespace std;
int main(){
    cout<<"in echo"<<endl;
    while(1){
        sleep(2);
        char msg[10]="From echo";
        send(2,msg,sizeof(msg),0);
    }
}