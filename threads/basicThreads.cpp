#include<bits/stdc++.h>
#include<pthread.h>
using namespace std;
void* func1(void* args){
    int n=5;
    while(n--){
        sleep(2);
        cout<<"func1"<<endl;
    }
}
void* func2(void* args){
    int n=3;
    while(n--){
        string s;
        cin>>s;
        cout<<s<<endl;
    }
}
int main(){
    pthread_t th1,th2;
    pthread_create(&th1,NULL,func1,NULL);
    pthread_create(&th2,NULL,func2,NULL);
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    cout<<"Main ends"<<endl;
}