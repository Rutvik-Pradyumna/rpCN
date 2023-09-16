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

    mkfifo("name5", 0666);

    int fd = open("name5", O_RDONLY);

    char buff[200];

    int sz;

    while ((sz = read(fd, buff, sizeof(buff))) > 0)
    {

        buff[sz] = '\0';

        printf("%s\n", buff);

        fflush(stdout);
    }

    close(fd);
}