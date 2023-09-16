#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;

int main(){
	mkfifo("mkfifoTest",0666);
	int rfd=open("mkfifoTest",O_RDONLY);
	char c;
	while(read(rfd,&c,sizeof(c))>0) cout<<c;
	cout<<endl;
}
