#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<linux/ip.h>
using namespace std;

int main(){
    int rsfd=socket(AF_INET,SOCK_RAW,15); 
    if(rsfd<0){
        perror("socket err");
        return 0;
    }

    int fd=open("content.txt",O_RDONLY);
    dup2(fd,0);

    struct sockaddr_in caddr;
    caddr.sin_family=AF_INET;
    caddr.sin_addr.s_addr=inet_addr("127.5.5.5");
    if(connect(rsfd,(struct sockaddr*)&caddr,sizeof(caddr))<0) perror("connect");

    dup2(rsfd,1);
    execlp("./s2","s2",NULL);
}