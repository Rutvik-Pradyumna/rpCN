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

struct message
{

    long type;

    char txt[100];
};

int main()
{

    system("touch tochatserver.txt");

    system("touch fromchatserver.txt");

    key_t tokey = ftok("tochatserver.txt", 'B');

    key_t fromkey = ftok("fromchatserver.txt", 'B');

    long cno;

    int msqid1 = msgget(tokey, 0644 | IPC_CREAT);

    int msqid2 = msgget(fromkey, 0644 | IPC_CREAT);

    while (1)
    {

        for (int i = 0; i < 10; i++)
        {

            struct message msg;

            int st = msgrcv(msqid1, &msg, sizeof msg, i + 1, 0 | IPC_NOWAIT);

            if (st == -1)
            {

                continue;
            }
            else
            {

                printf("from %i %ld: %s", i + 1, msg.type, msg.txt);

                for (int i = 0; i < 10; i++)
                {

                    if ((i + 1) != msg.type)
                    {

                        struct message smsg;

                        smsg.type = (long)(11 * (i + 1)) + msg.type;

                        strcpy(smsg.txt, msg.txt);

                        msgsnd(msqid2, &smsg, sizeof smsg, 0);
                    }
                }
            }
        }
    }
}