#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;

int main(){
	int ifd=open("dupFile.txt",O_RDONLY);
	int tfd=dup(0);
	dup2(ifd,0);
	int c=fork();
	if(c>0){
		cout<<"Parent"<<endl;
		close(ifd);
	}
	else{
		cout<<"child"<<endl;
		execlp("./dupP2","dupP2",NULL);
	}
	return 0;
}
