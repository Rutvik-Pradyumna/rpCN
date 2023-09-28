#include <stdio.h>

#include <unistd.h>

#include <fcntl.h>

int main()
{

    int c = 0;

    int fd = open("ccode.txt", O_RDONLY);

    c = fork();

    if (c > 0)
    {
        wait(NULL);
        char c[10];

        int sz = read(fd, c, 10);

        c[sz] = '\0';

        printf("%s", c);

        close(fd);

        printf("parent end\n");
    }
    else
    {
        // sleep(1);
        char c[10];

        printf("child start\n");

        int sz = read(fd, c, 10);

        c[sz] = '\0';

        printf("%s", c);

        printf("child end\n");
    }
}