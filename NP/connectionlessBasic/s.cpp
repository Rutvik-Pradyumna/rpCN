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

void setSockOpt(int &sfd){
    int opt=1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0){
        perror("sockopt err");
        exit(1);
    }
}

void bindSocket(int &sfd,int port){
    sAddr.sin_family=AF_INET;
    sAddr.sin_addr.s_addr=inet_addr("127.8.8.8");
    sAddr.sin_port=htons(port);
    if(bind(sfd, (struct sockaddr*)&sAddr, adrlen)<0){
        perror("bind err");
        exit(1);
    }
}

int main(){
    int sfd;
    createSocket(sfd);
    setSockOpt(sfd);
    bindSocket(sfd,8080);
    char buff[50];
    ssize_t sz=recvfrom(sfd,buff,50,0,(struct sockaddr*)&cAddr,(socklen_t*)&adrlen);
    buff[sz]='\0';
    cout<<"Recieved "<<buff<<endl;
    string s="From Server";
    strcpy(buff,s.c_str());
    sendto(sfd,buff,sizeof(buff),0,(struct sockaddr*)&cAddr,adrlen);
    cout<<"Sent"<<endl;
    close(sfd);
}