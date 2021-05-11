#include<stdio.h>
#include<signal.h>
void 
print(int signo)
{
    printf("resive sigint\n");
}

int
main(void)
{
    struct sigaction act_old;
    //if(signal(SIGINT,print)==SIG_DFL)
    //    printf("SIG_DFL\n");   能够打印出，返回的是就是上一次的处理函数
    sigaction(SIGINT,NULL,&act_old);
    if(act_old.sa_handler==SIG_DFL)  //返回的也是上次的处理函数
        printf("sig_dfl\n");   //如果第二个参数是NULL的话，不会修改信号的处理方式。也就是说，当发生sigint信号的时候还是会终止进程
    while(1)
        ;

    return 0;
}
