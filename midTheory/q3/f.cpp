#include<bits/stdc++.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<poll.h>
using namespace std;

struct sockaddr_in addr,cAddr;
int adrlen=sizeof(addr);
vector<pair<int,int>> pids; // {port,pid}

void createTCPSocket(int &sfd){
    if((sfd=socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("tcp socket err");
        exit(1);
    }
}

void createUDPSocket(int &sfd){
    if((sfd=socket(AF_INET, SOCK_DGRAM, 0))<0){
        perror("udp socket err");
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
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port=htons(port);
    if(bind(sfd, (struct sockaddr*)&addr, adrlen)<0){
        perror("bind err");
        exit(1);
    }
}

void listenSocket(int &sfd){
    if(listen(sfd,3)<0){
        perror("listen err");
        exit(1);
    }
}

void handler(int sigNum,siginfo_t *info,void *context){
    cout<<"Added "<<info->si_pid<<endl;
    pids.back().second=info->si_pid;
}

int main(){
    struct sigaction sa;
    sa.sa_sigaction=handler;
    sa.sa_flags=SA_SIGINFO;
    sigaction(SIGUSR1,&sa,NULL);

    int usfd;
    createUDPSocket(usfd);
    setSockOpt(usfd);
    bindSocket(usfd,8080);
    struct pollfd pfd[100];
    int pollsz=2;
    pfd[0].fd=0; pfd[1].fd=usfd;
    for(int i=0;i<100;i++) pfd[i].events=POLLIN;
    int k=0;
    while(1){
        poll(pfd,pollsz,-1);
        if(pfd[0].revents & POLLIN){
            char buff[100]={'\0'};
            read(0,buff,sizeof(buff));
            int sz=strlen(buff);
            string port="",fileName="";
            int i=0;
            while(buff[i]!=',') port.push_back(buff[i++]);
            int newPort=stoi(port);
            pids.push_back({newPort,1});
            i++;
            while(i<sz-1) fileName.push_back(buff[i++]);
            int sifd;
            createTCPSocket(sifd);
            setSockOpt(sifd);
            bindSocket(sifd,newPort);
            listenSocket(sifd);
            pfd[pollsz++].fd=sifd;
            int c=fork();
            if(c==0){
                dup2(sifd,2);
                execlp(("./"+fileName).c_str(),(fileName).c_str(),NULL);
            }
        }
        else if(pfd[1].revents & POLLIN){
            char buff[100]={'\0'};
            recvfrom(pfd[1].fd,buff,sizeof(buff),0,(struct sockaddr*)&cAddr,(socklen_t*)&adrlen);
            // send ports of all servers - Si
            string pidMsg="";
            for(auto p:pids){
                pidMsg += to_string(p.first)+" ";
            }
            sendto(pfd[1].fd,pidMsg.c_str(),pidMsg.size(),0,(struct sockaddr*)&cAddr,adrlen);
            cout<<"From "<<inet_ntoa(addr.sin_addr)<<" "<<ntohs(cAddr.sin_port)<<" "<<pidMsg<<endl;
        }
        else{
            for(int i=2;i<pollsz;i++){
                if(pfd[i].revents & POLLIN){
                    kill(pids[i-2].second,SIGUSR1);
                    sleep(2);
                }
            }
        }
    }
}