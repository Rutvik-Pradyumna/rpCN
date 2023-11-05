#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
using namespace std;

struct sockaddr_in sAddr;
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
    sAddr.sin_addr.s_addr = inet_addr("127.8.8.8"); 
    string msg="From Client";
    sendto(sfd,msg.c_str(),msg.size(),0,(struct sockaddr*)&sAddr,adrlen);
    cout<<"Sent"<<endl;
    char buff[50];
    ssize_t sz=recvfrom(sfd,buff,50,0,(struct sockaddr*)&sAddr,(socklen_t*)&adrlen);
    buff[sz]='\0';
    cout<<"Recieved "<<buff<<endl;
    close(sfd);
}