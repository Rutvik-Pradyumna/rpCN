#include<bits/stdc++.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/un.h>
using namespace std;
#define PATH "hehe"
int main(){
    int usfd;
    struct sockaddr_un sAddr, cAddr, addr;
    int adrlen=sizeof(sAddr);

    usfd=socket(AF_UNIX, SOCK_STREAM, 0);
    sAddr.sun_family=AF_UNIX;
    unlink(PATH);
    strcpy(sAddr.sun_path,PATH);
    if(bind(usfd,(struct sockaddr*)&sAddr,adrlen)<0){
        perror("bind");
        exit(0);
    }
    cout<<"Bind Success"<<endl;
    listen(usfd, 5);
    int nusfd=accept(usfd,(struct sockaddr*)&cAddr,(socklen_t*)&adrlen);
    if(nusfd<0){
        perror("accept ");
        exit(0);
    }
    cout<<"Accepted"<<endl;
    // getsockname(usfd,(struct sockaddr*)&addr,(socklen_t*)&adrlen);
    // cout<<"path "<<(addr.sun_path)<<endl;

    // getsockname(nusfd,(struct sockaddr*)&addr,(socklen_t*)&adrlen);
    // cout<<"path "<<(addr.sun_path)<<endl;

    // getpeername(nusfd,(struct sockaddr*)&addr,(socklen_t*)&adrlen);
    // cout<<"path "<<(addr.sun_path)<<endl;
    while(1){
        sleep(1);
        char buff[50]="from server";
        send(nusfd,buff,sizeof(buff),0);
        buff[50]={'\0'};
        recv(nusfd,buff,sizeof(buff),0);
        cout<<"reply "<<buff<<endl;
    }
}