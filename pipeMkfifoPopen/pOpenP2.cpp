#include<bits/stdc++.h>
#include<unistd.h>
#include<fcntl.h>
using namespace std;

int main(){
	string s="Into pipe by P2";
	write(1,s.c_str(),s.size());
}
/*
// part-1 : P1 writes into pipe and P2 reads it with 0(input-fd)
cout<<"In P2"<<endl;
char c;
while(read(0,&c,sizeof(c))>0) cout<<c;
cout<<"\nP2 ended"<<endl;
*/

/*
// part-2 : P1 reads what P2 writes into pipe(actually into terminal)
	string s="Into pipe by P2";
	write(1,s.c_str(),s.size());
*/
