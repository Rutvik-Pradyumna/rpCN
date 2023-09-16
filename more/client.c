#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/wait.h>
#include<pthread.h>

struct message
{
    long type;
    char txt[100];
};
long cno;
int msqid1,msqid2;

void* sendMsg(void* args){
    struct message msg;
    while(1){
        int sz = read(0, msg.txt, sizeof msg.txt);
        msg.txt[sz] = '\0';
        printf("Sent %s", msg.txt);
        msg.type = cno;
        msgsnd(msqid1, &msg, sizeof msg, 0);
    }
}

void* receiveMsg(void* args){
    while(1){
        for (int i = 0; i < 10; i++)
        {
            if (i + 1 != cno)
            {
                long mtype = 11 * (cno) + (long)(i + 1);
                struct message msg2;
                int st = msgrcv(msqid2, &msg2, sizeof(msg2), mtype, 0 | IPC_NOWAIT);
                if (st == -1)
                {
                    continue;
                }
                else
                {
                    if (msg2.txt)
                        printf("rcvd from %i : %s", i + 1, msg2.txt);
                }
            }
        }
    }
}

int main()
{
    system("touch tochatserver.txt");
    system("touch fromchatserver.txt");
    key_t tokey = ftok("tochatserver.txt", 'B');
    key_t fromkey = ftok("fromchatserver.txt", 'B');
    msqid1 = msgget(tokey, 0644 | IPC_CREAT);
    msqid2 = msgget(fromkey, 0644 | IPC_CREAT);
    printf("Client Number : ");
    scanf("%ld", &cno);

    pthread_t pth[2];
    pthread_create(&pth[0],NULL,sendMsg,NULL);
    pthread_create(&pth[0],NULL,receiveMsg,NULL);
    pthread_join(pth[0],NULL);
    pthread_join(pth[1],NULL);
}