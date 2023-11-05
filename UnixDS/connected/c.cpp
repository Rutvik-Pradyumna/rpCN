#include<bits/stdc++.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;
#define PATH "hehe"
int main(){
    int usfd;
    struct sockaddr_un sAddr;
    int adrlen=sizeof(sAddr);

    usfd=socket(AF_UNIX, SOCK_STREAM, 0);
    sAddr.sun_family=AF_UNIX;
    strcpy(sAddr.sun_path,PATH);
    if(connect(usfd,(struct sockaddr*)&sAddr,adrlen)<0){
        perror("connect");
        exit(1);
    }
    cout<<"Connected"<<endl;
    while(1){
        
        char buff[50]={'\0'};
        recv(usfd,buff,sizeof(buff),0);
        cout<<"served "<<buff<<endl;
        strcpy(buff,"from client");
        send(usfd,buff,sizeof(buff),0);
    }
}