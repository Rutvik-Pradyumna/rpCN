#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;
int main(){
    int rsfd=socket(AF_INET,SOCK_RAW,50);
    char *buff="From sender to reciever";

    struct sockaddr_in cAddr,sAddr;

    cAddr.sin_family=AF_INET;
    if(inet_pton(AF_INET, "127.6.6.1", &cAddr.sin_addr)<=0){
        perror("Error in address");
        return 0;
    }
    
    sAddr.sin_family=AF_INET;
    if(inet_pton(AF_INET, "127.1.1.1", &sAddr.sin_addr)<=0){
        perror("Error in address");
        return 0;
    }

    int st=sendto(rsfd,buff,strlen(buff),0,(struct sockaddr*)&cAddr,sizeof(cAddr));
    if(st<0){
        perror("send");
        return 0;
    }
    cout<<"Msg sent"<<endl;
}