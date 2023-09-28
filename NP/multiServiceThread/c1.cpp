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
    int port;
    cout<<"Enter port : ";
    cin>>port;
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(port);
    if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr)<=0){
        perror("Error in address");
        return 0;
    }
    if(connect(sfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
        perror("Error in connect");
        return 0;
    }
    cout<<"Connection successfull"<<endl;

    char c;
    cin>>c;
    send(sfd,&c,sizeof(c),0);
    cout<<"sfd "<<sfd<<endl;
    while(1){
        char buff[500];
        recv(sfd,buff,sizeof(buff),0);
        cout<<buff<<endl;
        cout.flush();
    }
    cout<<"Ends"<<endl;
}