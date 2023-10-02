#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;

struct sockaddr_in sAddr,cAddr;
int adrlen=sizeof(sAddr);

void createSocket(int &sfd){
    if((sfd=socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("socket err");
        exit(1);
    }
}

int main(){
    int sfd;
    createSocket(sfd);
    sAddr.sin_family = AF_INET; 
    sAddr.sin_port = htons(8080); 
    sAddr.sin_addr.s_addr = INADDR_ANY;
    string msg="From Client";
    sendto(sfd,msg.c_str(),msg.size(),0,(struct sockaddr*)&sAddr,adrlen);
    getsockname(sfd,(struct sockaddr*)&cAddr,(socklen_t*)&adrlen);
    cout<<"My port "<<htons(cAddr.sin_port)<<endl;
    char buff[100]={'\0'};
    recvfrom(sfd,buff,sizeof(buff),0,(struct sockaddr*)&sAddr,(socklen_t*)&adrlen);
    cout<<buff<<endl;

    cout<<"Enter a port : ";
    int port;
    cin>>port;
    sAddr.sin_port = htons(port); 
    if(inet_pton(AF_INET, "127.0.0.1", &sAddr.sin_addr)<=0){
        perror("Error in address");
        return 0;
    }
    int mainSfd=socket(AF_INET, SOCK_STREAM, 0);
    if(connect(mainSfd,(struct sockaddr*)&sAddr,adrlen)<0){
        perror("err in connect");
    }
    while(1){
        char buff[100]={'\0'};
        recv(mainSfd,buff,sizeof(buff),0);
        cout<<buff<<endl;
    }
}