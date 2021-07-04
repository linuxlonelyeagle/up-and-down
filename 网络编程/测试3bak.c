#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int a=1;
pthread_mutex_t mutex;
pthread_cond_t cond;
 static long cnt =0;
void *thread(void *vargp)
{
    long i,niters = *((long*)vargp);
    for(i = 0;i<niters;i++)
    {
        
        pthread_mutex_lock(&mutex);
        while(a==0)
        pthread_cond_wait(&cond,&mutex);
        a--;
        cnt++;
        a++;
      if(cnt==1000){
            pthread_detach(pthread_self());
            pthread_mutex_unlock(&mutex);
            pthread_cond_broadcast(&cond);   
        }
    else 
    {
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond);
    }
    }
}

int
main(int argc,char *argv[])
{

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    long niters;
    pthread_t tid1,tid2;
  //  sem_init(&sem,0,1);
    if(argc!=2)
    {
        printf("usage:%s <niters>\n",argv[0]);
        exit(0);
    }

    niters = atoi(argv[1]);
    pthread_create(&tid1,NULL,thread,&niters);
    pthread_create(&tid2,NULL,thread,&niters);
    while(1)
    {
    //if(cnt !=(2*niters))
    //    printf("BOOM! cnt =%ld\n",cnt);
    if(cnt == (2*niters))
    {
        printf("OK cnt=%ld\n",cnt);
        break;
        
    }
    }
    return 0;
}
