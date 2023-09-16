#include<bits/stdc++.h>
#include<fcntl.h>
using namespace std;
int main(){
	int fd=open("./GDB/serverPOutput1.txt",O_RDONLY);
	char c;
	while(read(fd,&c,sizeof(c))>0) cout<<(int)c<<endl;
	

	// int expOtfd1=open("./GDB/ot.txt",O_RDONLY);
	// int pOtfd1=open("./GDB/serverPOutput1.txt",O_RDONLY);
	// char k1,k2;
	// while((read(expOtfd1,&k1,sizeof(k1)))>0 && (read(pOtfd1,&k2,sizeof(k2)))>0){
	// 	cout<<k1<<" "<<k2<<endl;
	// }
	// close(expOtfd1); close(pOtfd1);
	// cout<<"end"<<endl;
}
