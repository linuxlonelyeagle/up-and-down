#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

int
main(void)
{
    umask(0);
    if(mknod("fifo",S_IFIFO|0666,0)==-1)
    {
        perror("mknod error!\n");
    }
    return 0;
}