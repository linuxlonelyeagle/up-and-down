#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


#define MAXBUF 1000
struct
{
pthread_mutex_t mutex;
int buf[MAXBUF];
int nput;
int nval;
}shared={PTHREAD_MUTEX_INITIALIZER};

struct 
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}full={PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER};

struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}empty={PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER};

void *produce(void*);
void *cumsume(void*);

int
main(void)
{
    pthread_t prod,cums;
    pthread_create(&prod,NULL,produce,NULL);
    pthread_create(&cums,NULL,cumsume,NULL);
    pthread_join(prod,NULL);
    pthread_join(cums,NULL);
    return 0;
}
void*
produce(void *arg)
{
    //还是生产
    for(;;)
    {
        pthread_mutex_lock(&shared.mutex);
        shared.buf[shared.nput]=shared.nval;
        shared.nput++;
        shared.nval++;
        if(shared.nput==1)
        pthread_cond_signal(&empty.cond);
        pthread_mutex_unlock(&shared.mutex);
        if(shared.nput+1==MAXBUF)
        {
            //等待
            pthread_mutex_lock(&full.mutex);
            pthread_cond_wait(&full.cond,&full.mutex);
            pthread_mutex_unlock(&full.mutex);
            
        }
    }
    return NULL;        
}
void *
cumsume(void *arg)
{
    for(;;)
    {
        pthread_mutex_lock(&shared.mutex);
        if(shared.nput<MAXBUF&&shared.nput!=0)
        {
            for(int i=0;i<=shared.nput;i++)
            {
                if(shared.buf[i]!=-1)
                {
                    printf("get shared.buf[%d] = %d\n",i,shared.buf[i]);
                    shared.buf[i]=-1;
                }
            }      
        }
        if(shared.nput+1==MAXBUF)   //重新开始生产
        {
            shared.nput=0;
            shared.nval=0;
            pthread_cond_signal(&full.cond);  //唤醒
        }
        pthread_mutex_unlock(&shared.mutex);
        //这个缓冲区是一个空的
        pthread_mutex_lock(&empty.mutex);
        if(shared.nput==0)
        {
            pthread_cond_wait(&empty.cond,&empty.mutex);
        }
        pthread_mutex_unlock(&empty.mutex);

    }
}