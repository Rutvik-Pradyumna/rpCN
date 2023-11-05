#include<bits/stdc++.h>
#include<sys/socket.h>      // for socket usage
#include<arpa/inet.h>       // for htons(), inet_pton()
using namespace std;

int main(){
    // creating socket
    int sfd;
    if((sfd = socket(AF_INET,SOCK_STREAM,0))<0){
        cout<<"Error in socket()"<<endl;
        return 0;
    }

    // setting opts to socket
    int opt=1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0){
        cout<<"Error in setsockopt()"<<endl;
        return 0;
    }

    // binding
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.8.8.8");
    if(bind(sfd,(struct sockaddr*)&serverAddress,sizeof(serverAddress))<0){
        perror("Error in bind()");
        return 0;
    }

    // listening
    if(listen(sfd,5)<0){
        cout<<"Error in listen()"<<endl;
        return 0;
    }

    // accepting
    int nsfd;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr); // must initialize like this else error
    if((nsfd = accept(sfd,(struct sockaddr*)&clientAddr,(socklen_t*)&clientAddrLen))<0){
        perror("Error in accept()");
        return 0;
    }
    cout<<"Accepted"<<endl;
    // recieving
    char buff[500];
    int sz;
    if((sz=recv(nsfd,buff,sizeof(buff),0))<0){
        perror("Error in recv");
        return 0;
    }
    buff[sz]='\0';
    cout<<buff<<endl;

    // sending
    // sleep(5);
    char *msg="From Server";
    if(send(nsfd,msg,strlen(msg),0)<0){
        perror("Error in Send");
        return 0;
    }
    cout<<"Connection successfull"<<endl;

    sockaddr_in addr;
    int adrlen=sizeof(addr);
    getsockname(sfd,(struct sockaddr*)&addr,(socklen_t*)&adrlen);
    cout<<"address "<<inet_ntoa(addr.sin_addr)<<endl;
    cout<<"port "<<ntohs(addr.sin_port)<<endl;

    getsockname(nsfd,(struct sockaddr*)&addr,(socklen_t*)&adrlen);
    cout<<"address "<<inet_ntoa(addr.sin_addr)<<endl;
    cout<<"port "<<ntohs(addr.sin_port)<<endl;

    getpeername(nsfd,(struct sockaddr*)&addr,(socklen_t*)&adrlen);
    cout<<"address "<<inet_ntoa(addr.sin_addr)<<endl;
    cout<<"port "<<ntohs(addr.sin_port)<<endl;

}