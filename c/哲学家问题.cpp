#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
struct chopsiticks
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int flag;
};
struct chopsiticks chopsitick[5]={{PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER},{PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER},
    {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER},{PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER},
    {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER}};
int count;   //用于记录哲学家同时拿筷子人数
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
void *start(void*);
int
main(void)
{
    pthread_setconcurrency(6);
    pthread_t phil[5];  //设置5个哲学家
        pthread_create(&phil[0],NULL,start,(void*)0);
        pthread_create(&phil[1],NULL,start,(void*)1);
        pthread_create(&phil[2],NULL,start,(void*)2);
        pthread_create(&phil[3],NULL,start,(void*)3);
        pthread_create(&phil[4],NULL,start,(void*)4);

    for(int i=0;i<1;i++)
    pthread_join(phil[i],NULL);
    return 0;
}
void*
start(void *arg)
{
    for(;;)
    {
    int i=(long)arg;
    int left =0,right=0;
    
    if(count<=4)
    { 
        pthread_mutex_lock(&mutex);
        count++; 
        pthread_mutex_lock(&chopsitick[i].mutex);
        if(chopsitick[i].flag==1)
        pthread_cond_wait(&chopsitick[i].cond,&chopsitick[i].mutex);
        printf("哲学家%d活得%d号筷子\n",i,i);
        chopsitick[i].flag=1;   //活得左边的筷子
        left=1;
        
        pthread_mutex_unlock(&chopsitick[i].mutex);
       pthread_mutex_unlock(&mutex);
    }   
    //获取右边的筷子
    pthread_mutex_lock(&chopsitick[(i+1)%5].mutex);
    if(chopsitick[(i+1)%5].flag==1)
    pthread_cond_wait(&chopsitick[(i+1)%5].cond,&chopsitick[(i+1)%5].mutex);
        chopsitick[(i+1)%5].flag=1;
        right=1;
        printf("哲学家%d活得%d号筷子\n",i,(i+1)%5);
    pthread_mutex_unlock(&chopsitick[(i+1)%5].mutex);
    if(left==1&&right==1)
    {   left=0;
        right=0;
        pthread_mutex_lock(&mutex);
        count--; 
        printf("社学家%d吃饭:\n",i);
        printf("哲学家%d开始思考\n",i);
        pthread_mutex_lock(&chopsitick[i].mutex);
        chopsitick[i].flag=0;
        printf("哲学家%d放下%d号筷子\n",i,i);
        pthread_cond_signal(&chopsitick[i].cond);
        pthread_mutex_unlock(&chopsitick[i].mutex);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&chopsitick[(i+1)%5].mutex);
        chopsitick[(i+1)%5].flag=0;
        printf("哲学家%d放下%d号筷子\n",i,(i+1)%5);
        pthread_cond_signal(&chopsitick[(i+1)%5].cond);
        pthread_mutex_unlock(&chopsitick[(i+1)%5].mutex);
        sleep(5);
    
    }
    }
}
