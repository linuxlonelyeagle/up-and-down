#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<ctype.h>

#define MAXLINE 256
int
main(void)
{
    int c;
    while((c=getchar())!=EOF)
    {
        if(isupper(c))
        c = tolower(c);
        if(putchar(c)==EOF)
        {
            printf("output error\n");
        }
        if(c == '\n')
        fflush(stdout);
    }
    exit(0);
}