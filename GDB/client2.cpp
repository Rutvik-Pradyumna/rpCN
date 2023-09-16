#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
using namespace std;
int main(){
	mkfifo("ServerFifo2",0666);
    cout<<"Client2 Opened mkfifo"<<endl;
    int wCodefd=open("ServerFifo2",O_WRONLY);
    int codefd=open("p2.cpp",O_RDONLY);
    char c;

    while(read(codefd,&c,sizeof(c))>0) write(wCodefd,&c,sizeof(c));
    close(codefd); close(wCodefd);
    // output
    wCodefd=open("ServerFifo2",O_RDONLY);
    while(read(wCodefd,&c,sizeof(c))>0) cout<<c;
    cout<<"\nClient2 exited"<<endl;
}