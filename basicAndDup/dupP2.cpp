#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;

int main(){
	cout<<"P2"<<endl;
	char c;
	while(read(0,&c,sizeof(c))>0){
		cout<<c;
	}
	cout<<endl;
	return 0;
}
