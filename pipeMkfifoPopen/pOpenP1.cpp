#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
using namespace std;

int main(){
// part 2 with pOpen
//	FILE* fd=popen("./pOpenP2","r");
//	char c;
//	while(fread(&c,sizeof(c),1,fd)>0) cout<<c;
//	cout<<"\nExit"<<endl;
//	pclose(fd);
}
/*
// part-1 : P1 writes into pipe and P2 reads it with 0(input-fd)
int pp[2];
pipe(pp);
int c=fork();
if(c>0){
	close(pp[0]);
	string s="Into pipe by P1";
	write(pp[1],s.c_str(),s.size());
	close(pp[1]);
	cout<<"Parent Exited"<<endl;
}
else{
	cout<<"In forked Child"<<endl;
	close(pp[1]);
	dup2(pp[0],0);
	execlp("./pOpenP2","pOpenP2",NULL);
}
*/


/*
// part-2 : P1 reads what P2 writes into pipe(actually into terminal)
	int pp[2];
	pipe(pp);
	int c=fork();
	if(c>0){
		wait(NULL);
		cout<<"In Parent"<<endl;
		close(pp[1]);
		char c;
		while(read(pp[0],&c,sizeof(c))) cout<<c;
		close(pp[0]);
		cout<<"\nParent ends"<<endl;
	}
	else{
		cout<<"In Child"<<endl;
		close(pp[0]);
		dup2(pp[1],1);
		execlp("./pOpenP2","pOpenP2",NULL);
	}
*/
