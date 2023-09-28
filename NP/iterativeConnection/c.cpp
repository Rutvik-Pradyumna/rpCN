#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;

int main(){
    int sfd;
    if((sfd = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("Error in socket()");
        return 0;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(8080);
    if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr)<=0){
        perror("Error in address");
        return 0;
    }
    if(connect(sfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
        perror("Error in connect");
        return 0;
    }

    // sending
    char *msg="From Client";
    if(send(sfd,msg,strlen(msg),0)<0){
        perror("Error in Send");
        return 0;
    }

    // recieving
    char buff[500];
    int sz;
    if((sz=recv(sfd,buff,sizeof(buff),0))<0){
        perror("Error in recv");
        return 0;
    }
    buff[sz]='\0';
    cout<<buff<<endl;

    cout<<"Connection successfull"<<endl;

    sockaddr_in addr;
    int adrlen=sizeof(addr);
    getsockname(sfd,(struct sockaddr*)&addr,(socklen_t*)&adrlen);
    cout<<"address "<<inet_ntoa(addr.sin_addr)<<endl;
    cout<<"port "<<ntohs(addr.sin_port)<<endl;
}