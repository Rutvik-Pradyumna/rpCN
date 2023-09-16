#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
using namespace std;
int main(){
	mkfifo("ServerFifo1",0666);
    cout<<"Client1 Opened mkfifo"<<endl;
    int wCodefd=open("ServerFifo1",O_WRONLY);
    int codefd=open("p1.cpp",O_RDONLY);
    char c;

    while(read(codefd,&c,sizeof(c))>0) write(wCodefd,&c,sizeof(c));
    close(codefd); close(wCodefd);
    // output
    wCodefd=open("ServerFifo1",O_RDONLY);
    while(read(wCodefd,&c,sizeof(c))>0) cout<<c;
    cout<<"\nClient1 exited"<<endl;
}