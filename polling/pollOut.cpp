#include<bits/stdc++.h>
#include<unistd.h>
#include<poll.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
int main(){
    mkfifo("pollOutFifo",0666);
    int rfd=open("pollOutFifo",O_RDONLY);
    char buff[1024];
    int byteRead;
    while((byteRead=read(rfd,buff,sizeof(buff)))>0){
        // buff[byteRead]='\0';
        cout<<buff<<endl;
        // cout.flush();
        // sleep(2);
    }
    close(rfd);
    cout<<"pollOut ended"<<endl;
}