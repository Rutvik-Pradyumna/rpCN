#include<bits/stdc++.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;
#define PATH "sockFile"
int main(){
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
    if(usfd<0) perror("socket");
    struct sockaddr_un myAddr, cAddr;
    int adrlen=sizeof(myAddr);
    unlink(PATH);
    myAddr.sun_family = AF_UNIX;
	strcpy(myAddr.sun_path, PATH);

    if(bind(usfd, (struct sockaddr *)&myAddr, adrlen)<0)
	perror("bind");

    char buff[10]={'\0'};
    recvfrom(usfd,buff,sizeof(buff),0,(struct sockaddr*)&myAddr,(socklen_t*)&adrlen);
    cout<<"got "<<buff<<endl;
    while(1);
}