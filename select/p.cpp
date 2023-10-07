#include<bits/stdc++.h>
using namespace std;
int main(){
    cout<<"Start..."<<endl;
    int pp[2];
    pipe(pp);
    int c=fork();
    if(c>0){
        cout<<"Parent"<<endl;
        close(pp[1]);
        FILE *pfile=popen("./p2","r");
        perror("popen");
        int pfd=fileno(pfile);
        cout<<pfd<<" "<<pp[0]<<endl;
        cout<<"enter maxFd : ";
        int maxFd;
        cin>>maxFd;
        while(1){
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(0,&rfds);    FD_SET(pfd,&rfds);    FD_SET(pp[0],&rfds);
            struct timeval timeout;
            timeout.tv_sec=0; timeout.tv_usec=0;
            int ret=select(maxFd+1,&rfds,NULL,NULL,NULL);
            cout<<"ret "<<ret<<endl;
            cout.flush();
            if(FD_ISSET(0,&rfds)){
                char buff[20]={'\0'};
                read(0,buff,sizeof buff);
                cout<<buff<<endl;
            }
            if(FD_ISSET(pp[0],&rfds)){
                char buff[20]={'\0'};
                read(pp[0],buff,sizeof buff);
                cout<<buff<<endl;
            }
            if(FD_ISSET(pfd,&rfds)){
                char buff[20]={'\0'};
                read(pfd,buff,sizeof buff);
                cout<<buff<<endl;
            }
        }
    }
    else{
        close(pp[0]);
        char buff[11]="From child";
        while(1){
            sleep(3);
            write(pp[1],buff,sizeof(buff));
        }
    }
}