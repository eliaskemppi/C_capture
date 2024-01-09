#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>


int capture(char filename[]) {
    int fd = open(filename, O_WRONLY|O_CREAT, 0777);//opening for writing (also creating if file doesnt exist)
    if (fd==-1){ //error handling
    return 1;
    }
    lseek(fd,0,SEEK_END); //writing to the end of the file
    dup2(fd,1); //copying the file desciptor over the stdout fd
    return 0;
}