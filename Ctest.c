#include<stdio.h>
#include<signal.h>       // to use signal system calls

void myHandler(int sigNo){
    printf("In Handler Function\n");
    // sleep(5);
}

int main(){
    // using signal()
    signal(SIGINT,myHandler);
    sleep(30);
    printf("In Main\n");
    // signal(SIGINT,SIG_DFL);
}