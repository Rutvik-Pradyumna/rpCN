#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void handler(int x)
{
    write(1,"This is handler of P1\n",sizeof "This is handler of P1\n"-1);
}
void handler2(int x)
{
    write(1,"This is handler of P2\n",sizeof "This is handler of P2\n"-1);
}

int main()
{
    signal(SIGUSR1,handler);
    setpgid(0,0);
    int gid=getpgid(0);
    int c=getpid();
	int c1=fork();
    if(c1>0)
    {
        int c2=fork();
        if(c2>0)
        {
            int c3=fork();
            if(c3>0)
            {
                int k;
            }
            else
            {
                setpgid(c3,c);
                while(1); 
            }
        }
        else
        {
            setpgid(c2,c);
            while(1); 
        }
    }
    else
    {
        signal(SIGUSR1,handler2);
        setpgid(c1,c);
        while(1); 
    }
    killpg(gid,SIGUSR1);
    while(1); 
    return 0;
}