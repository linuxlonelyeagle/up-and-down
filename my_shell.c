#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#define normal 0 /*一般的命令*/
#define out_redirent 1  /*输出重定向*/
#define in_redirent 2  /*输入重定向*/
#define have_pipe 3 /*命令中有管道*/

//打印shell的提示符
void print_prompt();
//得到输入的命令
void get_input(char *);
//对输入命令进行解析
void explanin_input(char *,int *,char a[][256]);
//执行命令
void do_cmd(int ,char [][256]);
//查找命令中的可执行程序
int find_command(char *);
void cd(char*);

int main()
{

    signal(SIGINT,SIG_IGN);
    int argcount = 0;
    char arglist[100][256];
    char **arg = NULL;
    char *buf = NULL;
    buf = (char *)malloc(256);
    if(buf == NULL)
    {
        perror("malloc failed");
        exit(-1);
    }
    while(1)
    {
        memset(buf,0,256);
        print_prompt();
        get_input(buf);   // buf是以\n结尾的 
        //若输出的命令为exit就推出程序
        if(0==strcmp(buf,"exit\n"))
            break;
        argcount=0;
        explanin_input(buf,&argcount,arglist);
        if(strcmp("cd",arglist[0])!=0)
        do_cmd(argcount,arglist);
        else if(strcmp("cd",arglist[0])==0)
            cd(arglist[1]);
    }
    if(buf !=NULL)
    {
        free(buf);
        buf=NULL;
    }
    exit(0);
}
void cd(char *path)
{
    chdir(path);
}
//打印出欢迎的界面
void print_prompt()
{
    printf("my_shell$$:\n");
}
//获取用户的输出
void get_input(char *buf)
{
    int len = 0;
    int ch;
    ch = getchar();
    while(len<256 && ch != '\n')
    {
        buf[len++]=ch;
        ch = getchar();
    }
    if(256 == len)
    {
        printf("command is too long \n");
        exit(-1);
    }
    buf[len] ='\n';
    len++;
    buf[len] = '\0';
}
void explanin_input(char *buf,int *argcount,char arglist[][256])
{
    char *p=buf;
    char *q=buf;
    int number = 0;
    while(1)
    {
        if(p[0]=='\n')
            break;
        if(p[0]==' ')
            p++;
        else
        {
            q=p;
            number = 0;
            while(q[0]!=' '&&(q[0]!='\n'))
            {
                number++;
                q++;
            }
            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number]='\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
}
void do_cmd(int argcount,char arglist[][256])
{
    //how 用于标识命令中是否含有> < |
    // background 标识命令中是否有后台运行标识符  &
    int flag=0,how=0,background=0,status,i,fd;
    char *arg[argcount+1];    
    char *argnext[argcount+1];
    char *file;
    pid_t  pid;
    //取出命令行
    for(i=0;i<argcount;i++)
        arg[i]=arglist[i];
    arg[i]=NULL;
    //查看命令行是否有后台运行
    for(i=0;i<argcount;i++)
    {
        if(strncmp(arg[i],"&",1)==0)
        {
           if(i==argcount-1)
           {
               background = 1;
                arg[argcount -1] = NULL;
                break;
           }
           else
           {
               printf("wrong command   %d\n",__LINE__);
               return;
           }
        }
    }
    for(i = 0;arg[i]!=NULL;i++)
    {
        if(strcmp(arg[i],">")==0)
        {
            flag++;
            how = out_redirent;
            if(arg[i+1]==NULL)
                flag++;
        }
        else if(strcmp(arg[i],"<")==0)
        {
            flag++;
            how = in_redirent;
            if(i==0)
                flag++;
        }
        else if(strcmp(arg[i],"|")==0)
        {
            flag++;
            how = have_pipe;
            if(arg[i+1]==NULL)
                flag++;
            if(i==0)
                flag++;
        }
    }
    //flag 大于 1 ,说明命令中含有多个>,<,|符号，本程序不支持这样的命令
    //或者格式不对。如 ls -l /temp >
    if(flag > 1)
    {
        printf("wrong command   %d\n",__LINE__);
        return ;
    }
    if(how==out_redirent)
    {
        for(i = 0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],">")==0)
            {
                file = arg[i+1];
                arg[i]=NULL;
            }
        }
    }
    if(how == in_redirent)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"<")==0)
            {
                file =arg[i+1];
                arg[i]=NULL;
            }
        }
    }
    if(how == have_pipe)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"|")==0)
            {
            arg[i] = NULL;
            int j;
            for(j = i+1;arg[j]!=NULL;j++)
            argnext[j-i-1] =arg[j];
            argnext[j-i-1] =arg[j];
            break;
            }
        }
        
    }
    if((pid = fork())<0)
    {
        printf("%d fork error\n",__LINE__);
        return;
    }
    if(pid==0)
    {
        //子进程设置信号
        signal(SIGINT,SIG_IGN);
    }
    switch(how)
    {
        case 0:
            if(pid==0) //子进程
            {
                if(!find_command(arg[0]))
                {
                  printf("%d %s:command not found\n",__LINE__,arg[0]);
                  exit(0);  
                }
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        case 1:
            //  >
            if(pid==0)
            {
                if(!(find_command(arg[0])))
                {
                    printf("%d %s :command not found\n",__LINE__,arg[0]);
                    exit(0);
                }
                fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0664);
                dup2(fd,1);
                execvp(arg[0],arg);
                exit(0);

            }
            break;


        case 2:
            //输出的命令中含有重定向符号 <
            if(pid == 0)
            {
                if(!(find_command(arg[0])))
                {
                    printf("%d %s :command not found\n",__LINE__,arg[0]);
                    exit(0);
                }
            
            fd = open(file,O_RDONLY);
            dup2(fd,0);
            execvp(arg[0],arg);
            exit(0);
            }
            break;
        case 3:
            if(pid == 0)   //子进程
            {
                int pid2,status2,fd2;
                if((pid2 = fork())<0)
                {
                    printf("%d fork2 error\n",__LINE__);
                    return;
                }
                if(pid2 ==0)
                {
                    if(!(find_command(arg[0])))
                    {
                        printf("%s: command not found %d \n",arg[0],__LINE__);
                        exit(0);
                    }
                    fd2 =open("/tmp/youdonotknowfile",O_WRONLY|O_CREAT|O_TRUNC,0664);
                    dup2(fd2,1);  //子进程负责输出
                    execvp(arg[0],arg);
                    exit(0);   //字进程退出
                }
                if(waitpid(pid2,&status2,0)==-1)
                {
                    printf("wait for child process error %d\n",__LINE__);
                }
                if(!(find_command(argnext[0])))
                {
                    printf("%s :command not found  %d\n",argnext[0],__LINE__);
                }
                fd2 = open("/tmp/youdonotknowfile",O_RDONLY);
                dup2(fd2,0);  //父进程负责输入
                if(remove("/tmp/youdonotknowfile"))                                                                                                                                     printf("remove error %d\n",__LINE__);
                execvp(argnext[0],argnext);
                exit(0);
            }
            break;
        default:
            break;

    }
    //若命令中有&，表示后台执行，父进程直接推出，不等待子进程结束
    if(background == 1)
    {
        printf("process id %d\n",pid);
        return;
    }
    //父进程等待子进程结束
   if(waitpid(pid,&status,0)==-1) 
    {
        printf("wait for child porcess error  %d\n",__LINE__);
    }

}
int find_command(char *command)
{

    DIR* dp;
    struct dirent* dirp;
    char *path[]={"./","/bin","/usr/bin",NULL};
    if(strncmp(command,"./",2)==0)
        command = command +2;
    int i = 0;
    while(path[i]!=NULL)
    {
        if((dp = opendir(path[i]))==NULL)
            printf("can not open /bin %d\n",__LINE__);
        while((dirp = readdir(dp))!=NULL)
        {
            if(strcmp(dirp->d_name,command)==0)
            {
                closedir(dp);
                return 1;
            }
        }
            closedir(dp);
            i++;
    }
    return 0;
}
