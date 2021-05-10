#include<unistd.h>
#include<stdio.h>
#include<sys/utsname.h>
#include<time.h>
#include<sys/times.h> //进程时间头文件
#include<stdlib.h>
#include<assert.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>
#include<setjmp.h>


void 
my_hander(int signo)
{
    printf("resive sigint\n");
}
int
main(int argc,char *argv[])
{
    signal(SIGINT,my_hander);
    int a=kill(getpid(),0);  //作用就是检查这个进程能不能向其他的进程组
    //和其他的进程发送信号
    printf("%d\n",a);
    return 0;
}
