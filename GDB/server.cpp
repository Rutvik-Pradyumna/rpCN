#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<poll.h>
using namespace std;
int main(){

    struct pollfd pfds[2];

    mkfifo("ServerFifo1",0666);
    int c1=open("ServerFifo1",O_RDONLY);
    pfds[0].fd=c1;
    pfds[0].events=POLLIN;

    mkfifo("ServerFifo2",0666);
    int c2=open("ServerFifo2",O_RDONLY);
    pfds[1].fd=c2;
    pfds[1].events=POLLIN;

    int count=0;
    char c;
    bool check;
    while(count<2){
        int ret=poll(pfds,2,-1);
        if(ret>0){
            for(int i=0;i<3;i++){
                // sleep(1);
                if(pfds[i].revents & POLLIN){
                    if(i==0){
                        cout<<"c1"<<endl;
                        int pCodefd1=open("serverPCode1.cpp",O_WRONLY | O_TRUNC);
                        while(read(c1,&c,sizeof(c))>0) write(pCodefd1,&c,sizeof(c));
                        close(c1);
                        close(pCodefd1);
                        system("g++ serverPCode1.cpp -o serverPCode1");

                        int pOtfd1=open("serverPOutput1.txt",O_WRONLY | O_TRUNC);
                        int infd1=open("it.txt",O_RDONLY);
                        int expOtfd1=open("ot.txt",O_RDONLY);
                        int t1fd1=dup(1);
                        int t0fd1=dup(0);
                        int cpid1=fork();
                        if(cpid1>0){
                            wait(NULL);
                            dup2(t1fd1,1); dup2(t0fd1,0);
                            close(t1fd1); close(t0fd1); close(pOtfd1);
                            cout<<"Server Parent 1"<<endl;

                            pOtfd1=open("serverPOutput1.txt",O_RDONLY | O_CREAT);
                            char k1,k2;
                            check=1;
                            while((read(expOtfd1,&k1,sizeof(k1)))>0 && (read(pOtfd1,&k2,sizeof(k2)))>0){
                                if (k1 == '\r' && k2=='\n')
                                    continue;
                                if(k1!=k2){
                                    check=0;
                                    break;
                                }
                            }
                            close(expOtfd1); close(pOtfd1);
                            int sendAns1=open("ServerFifo1",O_WRONLY);
                            string ans;
                            if(check) ans="Correct Ans";
                            else ans="Wrong Ans";
                            write(sendAns1,ans.c_str(),ans.size());
                            close(sendAns1); count++;
                        } else {
                            cout<<"Server Forked for 1"<<endl;
                            dup2(pOtfd1,1);
                            dup2(infd1,0);
                            execlp("./serverPCode1","serverPCode1",NULL);
                        }
                    }
                    else if(i==1){
                        cout<<"c2"<<endl;
                        int pCodefd2=open("serverPCode2.cpp",O_WRONLY | O_TRUNC);
                        while(read(c2,&c,sizeof(c))>0) write(pCodefd2,&c,sizeof(c));
                        close(c1);
                        close(pCodefd2);
                        system("g++ serverPCode2.cpp -o serverPCode2");

                        int pOtfd2=open("serverPOutput2.txt",O_WRONLY | O_TRUNC);
                        int infd2=open("it.txt",O_RDONLY);
                        int expOtfd2=open("ot.txt",O_RDONLY);
                        int t1fd2=dup(1);
                        int t0fd2=dup(0);
                        int cpid2=fork();
                        if(cpid2>0){
                            wait(NULL);
                            dup2(t1fd2,1); dup2(t0fd2,0);
                            close(t1fd2); close(t0fd2); close(pOtfd2);
                            cout<<"Server Parent 2"<<endl;

                            pOtfd2=open("serverPOutput2.txt",O_RDONLY | O_CREAT);
                            char k1,k2;
                            check=1;
                            while((read(expOtfd2,&k1,sizeof(k1)))>0 && (read(pOtfd2,&k2,sizeof(k2)))>0){
                                if (k1 == '\r' && k2=='\n')
                                    continue;
                                if(k1!=k2){
                                    check=0;
                                    break;
                                }
                            }
                            close(expOtfd2); close(pOtfd2);
                            int sendAns2=open("ServerFifo2",O_WRONLY);
                            string ans;
                            if(check) ans="Correct Ans";
                            else ans="Wrong Ans";
                            write(sendAns2,ans.c_str(),ans.size());
                            close(sendAns2); count++;
                        } else {
                            cout<<"Server Forked for 2"<<endl;
                            dup2(pOtfd2,1);
                            dup2(infd2,0);
                            execlp("./serverPCode2","serverPCode2",NULL);
                        }
                    }
                }
            }
        }
    }

    cout<<"\nServer Exited"<<endl;
}


/*
// v1
mkfifo("ServerFifo",0666);
    cout<<"Entered Server"<<endl;
    int readCodefd=open("ServerFifo",O_RDONLY);
    int pCodefd=open("serverPCode.cpp",O_WRONLY | O_TRUNC);
    char c;
    while(read(readCodefd,&c,sizeof(c))>0) write(pCodefd,&c,sizeof(c));
    close(readCodefd);
    close(pCodefd);
    system("g++ serverPCode.cpp -o serverPCode");

    int pOtfd=open("serverPOutput.txt",O_WRONLY | O_TRUNC);
    int infd=open("it.txt",O_RDONLY);
    int expOtfd=open("ot.txt",O_RDONLY);
    int t1fd=dup(1);
    int t0fd=dup(0);
    int cpid=fork();
    if(cpid>0){
        wait(NULL);
        dup2(t1fd,1); dup2(t0fd,0);
        close(t1fd); close(t0fd); close(pOtfd);
        cout<<"Server Parent"<<endl;

        pOtfd=open("serverPOutput.txt",O_RDONLY);
        char c1,c2;
        bool check=1;
        while((read(expOtfd,&c1,sizeof(c1)))>0 && (read(pOtfd,&c2,sizeof(c2)))>0){
            if (c1 == '\r' && c2=='\n')
                continue;
            if(c1!=c2){
                check=0;
                break;
            }
        }
        close(expOtfd); close(pOtfd);
        int sendAns=open("ServerFifo",O_WRONLY);
        string ans;
        if(check) ans="Correct Ans";
        else ans="Wrong Ans";
        write(sendAns,ans.c_str(),ans.size());
        close(sendAns);
    } else {
        cout<<"Server Forked"<<endl;
        dup2(pOtfd,1);
        dup2(infd,0);
        execlp("./serverPCode","serverPCode",NULL);
    }
*/