#include<bits/stdc++.h>
using namespace std;

void* func1(void *args){
    cout<<"func1"<<endl;
    int n=5;
    while(n--){
        system("./P1thfork");
        cout<<"hi"<<endl;
    }
}

void* func2(void *args){
    int n=5;
    while(n--){
        sleep(1);
        cout<<"func2"<<endl;
    }
}

int main(){
    pthread_t th[2];
    pthread_create(&th[0],NULL,func1,NULL);
    pthread_create(&th[1],NULL,func2,NULL);
    pthread_join(th[0],NULL);
    pthread_join(th[1],NULL);
    cout<<"main ends"<<endl;
    return 0;
}