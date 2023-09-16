#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;

int main(){
	int ofd=open("dupFile.txt",O_WRONLY | O_CREAT | O_APPEND,1024);
	int tfd=dup(1);
	dup2(ofd,1);
	cout<<"Into the dupFile 2nd time"<<endl;
	close(ofd);
	dup2(tfd,1);
	cout<<"In Terminal"<<endl;
	close(tfd);
	return 0;
}
