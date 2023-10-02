#include<bits/stdc++.h>
#include<fcntl.h>
#include<sys/wait.h>
using namespace std;
int main(){
    int t1fd=dup(1);
    // int fd=dup(0);
    int c=fork();
    if(c>0){
        while(1){
            sleep(2);
            cout<<"Parent1"<<endl;
        }
    }
    else{
        cout<<"Parent2"<<endl;
        // int fd=open("ccode.txt",O_WRONLY | O_TRUNC);
        if (dup2(0, 1) == -1) {
            perror("dup2");
            exit(1);
        }
        system("./p2");
        dup2(t1fd,1);
        // close(fd);
        // fd=open("ccode.txt",O_RDONLY);
        // dup2(fd,0);
        cout<<"p3ing"<<endl;
        execlp("./p3","/p3",NULL);
    }
    // int fd=open("ccode.txt",O_RDWR );
    // string s="mymsg";
    // char c;
    // while(read(fd,&c,sizeof(c))>0) cout<<c;
    // cout<<endl;
    // write(fd,s.c_str(),s.size());
    // while(read(fd,&c,sizeof(c))>0) cout<<c;

}