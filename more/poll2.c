#include<stdio.h>

#include<unistd.h>

#include<string.h>

#include<fcntl.h>

#include<sys/types.h>

#include<sys/stat.h>

int main(){

mkfifo("nameit",0666);

int fd=open("nameit",O_WRONLY);

char *buffer="here is from meee";

write(fd,buffer,strlen(buffer));

//close(fd);

}