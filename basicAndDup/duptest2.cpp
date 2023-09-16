#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;

int main(){
	int ifd=open("dupFile.txt",O_RDONLY);
	int tfd=dup(0);
	dup2(ifd,0);
	char c;
	while(cin.get(c)){
		cout<<c;
	}
	dup2(tfd,0);
	close(tfd);
	close(ifd);
	return 0;
}
