#include <stdio.h>

#include <unistd.h>

#include <string.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <sys/poll.h>

#include <sys/time.h>

#include <sys/wait.h>

int main()
{

    setvbuf(stdin, NULL, _IONBF, 0);

    int c = 0;

    int pp[2];

    pipe(pp);

    c = fork();

    if (c > 0)
    {

        close(pp[1]);

        mkfifo("nameit", 0666);

        int fd = open("nameit", O_RDONLY);

        struct pollfd pfds[3];

        pfds[0].fd = pp[0];

        pfds[0].events = POLLIN;

        pfds[2].fd = 0;

        pfds[2].events = POLLIN;

        pfds[1].fd = fd;

        pfds[1].events = POLLIN;

        int j = 0;

        mkfifo("name5", 0666);

        int wfd = open("name5", O_WRONLY);

        while (1)
        {

            j++;

            printf("%i\n", j);

            if (j > 20)
            {
                break;
            }

            int ret = poll(pfds, 3, -1);

            if (ret > 0)
            {

                for (int i = 0; i < 3; i++)
                {

                    if (pfds[i].revents & POLLIN)
                    {

                        char buffer[200];

                        int sz = read(pfds[i].fd, buffer, sizeof(buffer));

                        printf("size of buuf is %i\n", sz);

                        buffer[sz] = '\0';

                        printf("%s\n", buffer);

                        write(wfd, buffer, sz);

                        fsync(wfd);
                    }

                    sleep(1);
                }
            }
        }

        close(wfd);

        close(fd);
    }
    else
    {

        close(pp[0]);

        char *msg = "From child\n";

        write(pp[1], msg, strlen(msg));
    }
}