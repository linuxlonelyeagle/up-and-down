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

#define ENV_UNSAVE 0
#define ENV_SAVE 1

int count;
int flag_saveenv = ENV_UNSAVE;
jmp_buf env ;   // 保存跳转信息的栈信息
void 
handler_sigrtmin(int signo)
{
    if(flag_saveenv==ENV_UNSAVE)
        return;
    printf("recv SIGINT\n");
    sleep(5);
    printf("in SIGINT,after sleep");
    siglongjmp(env,1);  //第二个参数可以设置返回值.
}
int
main()
{
    printf("pid:%d\n",getpid());
    //设置返回点
    //这个env 是一个结构体.
    switch(sigsetjmp(env,1))
    {
        case 0:
            printf("count:%d\n",count++);
            printf("return 0\n");
            flag_saveenv=ENV_SAVE;
            break;
        case 1:
            printf("count:%d\n",count++);
            printf("return from SIGINT");
            break;
        default:
            printf("count:%d\n",count++);
            printf("return else\n");
            break;
    }
    //设置成为不可靠的信号的话，这个信号会丢失
    signal(SIGINT,handler_sigrtmin);

        while(1)
            ;
        return 0;
    return 0;
}
