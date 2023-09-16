#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;
int main(){
	int c=0,i=0;
	cout<<"Parent start"<<endl;
	c=fork();
	wait(NULL);
	i=1;
	c=fork();
	wait(NULL);
	i=2;
	if(c>0){
		cout<<"pid"<<c<<endl;
	}
	else{
		cout<<i<<"parent child1"<<endl;
		execlp("./basicTest2", "basicTest2", NULL);
	}
	cout<<"Exited"<<endl;
	return 0;
}
