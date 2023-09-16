#include<bits/stdc++.h>
#include<unistd.h>
#include<poll.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
int main(){
    struct pollfd pfds[3];
    int pp[2];
    pipe(pp);
    mkfifo("fifoP2",0666);
    int p2fd=open("fifoP2",O_RDONLY);
    pfds[0].fd=pp[0];
    pfds[0].events=POLLIN;
    pfds[1].fd=0;
    pfds[1].events=POLLIN;
    pfds[2].fd=p2fd;
    pfds[2].events=POLLIN;
    int c=fork();
    if(c>0){
        int count=0;
        while(count!=3){
            int ret=poll(pfds,3,-1);
            if(ret>0){
                sleep(1);
                for(int i=0;i<3;i++){
                    if(pfds[i].revents & POLLIN){
                        cout<<"i is : "<<i<<endl;
                        mkfifo("pollOutFifo",0666);
                        int wfd=open("pollOutFifo",O_WRONLY);
                        char buff[1024];
                        int bytesRead=read(pfds[i].fd,buff,sizeof(buff));
                        buff[bytesRead]='\0';
                        write(wfd,buff,sizeof(buff));
                        count++;
                        // cout.flush();
                    }
                }
            }
            // cout<<count<<endl;
        }
    }
    else{
        cout<<"In Child"<<endl;
        close(pp[0]);
        string s="Written by Child";
        write(pp[1],s.c_str(),s.size());
        close(pp[1]);
        cout<<"Child ends"<<endl;
    }
    return 0;
}

// part-1 just reading  couts from 3 files with popen
/*
struct pollfd pfds[4];
pfds[0].fd=fileno(popen("./pollChild1","r"));
pfds[0].events=POLLIN;
pfds[1].fd=fileno(popen("./pollChild2","r"));
pfds[1].events=POLLIN;
pfds[2].fd=fileno(popen("./pollChild3","r"));
pfds[2].events=POLLIN;
char buff[1024];
int count=4;
while(count--){
    int ret=poll(pfds,4,-1);
    if(ret>0){
        for(int i=0;i<3;i++){
            sleep(1);
            if(pfds[i].revents & POLLIN){
                int bytesRead=read(pfds[i].fd,buff,sizeof(buff));
                mkfifo("pollOutFifo",0666);
                int wfd=open("pollOutFifo",O_WRONLY);
                write(wfd,buff,bytesRead);
                close(wfd);
            }
        }
    }
    else return 0;
}
*/