#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
int main(){
    // part - 1
    // cout<<"This is poll-child1"<<endl;
    // cout.flush();


    // part - 2
    mkfifo("fifoP2",0666);
    int p2fd=open("fifoP2",O_WRONLY);
    string s="From pollChild1";
    write(p2fd,s.c_str(),s.size());
    close(p2fd);
    return 0;
}