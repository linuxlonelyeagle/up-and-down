#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<pthread.h>
void*
pthread(void *arg)
{
    pthread_detach(pthread_self());
    while(1)
        ;
    return 0;
}

int 
main(void)
{
    pthread_t pth;
    pthread_create(&pth,NULL,pthread,NULL);
    pthread_join(pth,NULL);
    printf("hello world");
    return 0;    
}
