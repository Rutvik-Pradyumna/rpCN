#include<bits/stdc++.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;
#define PATH "sockFile"
int main(){
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
    if(usfd<0) perror("socket");
    struct sockaddr_un myAddr;
    int adrlen=sizeof(myAddr);

    myAddr.sun_family = AF_UNIX;
	strcpy(myAddr.sun_path, PATH);

    char buff[10]="From c1";
    int sz=sendto(usfd,buff,sizeof(buff),0,(struct sockaddr*)&myAddr,adrlen);
    if(sz<0) perror("sent");
    cout<<"sent "<<sz<<endl;
    while(1);
}