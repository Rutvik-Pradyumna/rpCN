#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
int main(){
    mkfifo("fifo",0666);
    int fd=open("fifo",O_WRONLY);
    while(1){
        string s="From p2";
        write(fd,s.c_str(),s.size());
        sleep(5);
    }
}