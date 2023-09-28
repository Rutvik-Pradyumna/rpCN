#include <bits/stdc++.h> 
#include<sys/socket.h>
#include<sys/shm.h>
using namespace std;
struct shMemory{
    bool s1;
    bool s2;
};

int main()
{
	cout<<"In s1"<<endl;
	char buff[500];
	int sz;
	if((sz=recv(2,buff,sizeof(buff),0))<0){
	    perror("Error in recv1");
	    return 0;
	}
	buff[sz]='\0';
	cout<<buff;

	// updating shm
	key_t key=ftok("spoll.cpp",'A');
	int shmid=shmget(key,sizeof(shMemory),0666 | IPC_CREAT);
	shMemory *shmPtr=(struct shMemory*)shmat(shmid,NULL,0);
	shmPtr->s1=1;

	cout<<"\ns1 terminated"<<endl;
	return 0;
}