#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;
int main(){
	int pp[2];
	pipe(pp);
	int c=fork();
	if(c>0){
		close(pp[0]);
		string buff;
		getline(cin,buff);
		write(pp[1],buff.c_str(),buff.size());
		close(pp[1]);
		cout<<"Parent Exited"<<endl;
	}
	else{
		close(pp[1]);
		char c;
		while(read(pp[0],&c,sizeof(c))>0) cout<<c;
		close(pp[0]);
		cout<<"Child Exited"<<endl;
	}
	return 0;
}
