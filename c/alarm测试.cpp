#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<signal.h>

int
main(void)
{
    //每个进程只有一个闹钟时间
    time_t time =alarm(20);   //输出0
    printf("alarm1:%d\n",time);
    sleep(5);
    time = alarm(20); 
    printf("alarm:%d\n",time);   //输出15
    return 0;
}