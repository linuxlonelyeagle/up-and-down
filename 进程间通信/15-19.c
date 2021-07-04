#include<stdio.h>
#include<stdlib.h>

#define MAXLINE 256

int
main(void)
{
    //这样实际上是不行的，因为是全缓冲的，这样的话，只要缓冲区没满的花
    //数据就一直在缓冲区，所以读和输入都阻塞了
    //如果我们在while循环之前设置缓冲区的花，及不会出现这种问题了
    
    if(setvbuf(stdin,NULL,_IOLBF,0)!=0)
        printf("标准输入缓冲区设置失败\n");
    if(setvbuf(stdout,NULL,_IOLBF,0)!=0)
        printf("设置标准输出缓冲区失败\n");
    int int1,int2;
    char line[MAXLINE];

    while(fgets(line,MAXLINE,stdin)!=NULL)
    {
        if(sscanf(line,"%d%d",&int1,&int2)==2)
        {
            if(printf("%d\n",int1+int2)==EOF)
            {
                printf("printf error\n");
            }
        }
        else
        {
            if(printf("invalid args\n")==EOF)
            {
                printf("printf error\n");
            }
        }
    }
    return 0;
}
