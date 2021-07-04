#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#define DEF_PAGER  "/bin/more"  
#define MAXLINE 20

int 
main(int argc,char *argv[])
{

    int n;
    int fd[2];
    pid_t pid;
    char *pager,*argv0;
    char line[MAXLINE];
    FILE *fp;
    if(argc != 2)
    {
        printf("参数不等于2\n");
        exit(-1);
    }
    if((fp = fopen(argv[1],"r"))==NULL)    //fp是输入的第一个文件
    {
        printf("打开文件失败\n");
        exit(-1);        
    }
    if(pipe(fd)<0)
    {
        printf("pipe error\n");
        exit(-1);
    }
    if((pid = fork())<0)
    {
        printf("fork error");
        exit(-1);
    }
    else if(pid > 0)     
    {
        close(fd[0]); //父进程关闭读文件 

        while(fgets(line,MAXLINE,fp)!=NULL)
        {
            n =strlen(line);
            if(write(fd[1],line,n)!=n)    //向文件里面写入内容
            {
                printf("write error to pipe");
            }
        }
        if(ferror(fp))
        {
            printf("fgets error\n");
            exit(-1);
        }
        close(fd[1]);
        if(waitpid(pid,NULL,0)<0)        //等待子进程
        {
            printf("waitpid error\n");
            exit(-1);
        }
        exit(0);
    }
    else
    {
        close(fd[1]); //关闭写文件

        if(fd[0] != STDIN_FILENO)
        {
            if(dup2(fd[0],STDOUT_FILENO) != STDIN_FILENO)
            {
                printf("dup2 error to stdin\n");
                exit(-1);
            }
            close(fd[0]);   
        }
        //get arguments for execl
        if((pager = getenv("PAGER"))==NULL)
        pager = DEF_PAGER;
        if((argv0 == strrchr(pager,'/'))!=NULL)    //返回最后一个字符出现的位置,所以后面要自加
        argv0++;
        else
            argv0 = pager;  
        if(execl(pager,argv0,(char*)0)<0)
        {
            printf("execl error for %s\n",pager);
            exit(-1);
        }
        exit(0);
    }
    return 0;
}