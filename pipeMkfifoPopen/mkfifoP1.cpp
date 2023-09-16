#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;

int main(){
	mkfifo("mkfifoTest",0666);
	int n=10;
	// while(n--){
		int wfd=open("mkfifoTest",O_WRONLY);
		string s="Into pipe by P1\n";
		write(wfd,s.c_str(),s.size());
		close(wfd);
	// }
}
