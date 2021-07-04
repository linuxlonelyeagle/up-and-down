#include<stdio.h>
#include<pthread.h>
#define true 1
#define false 0
#define DEFAULT_TIME 1
#define DEFAULT_THREAD_NUM 1    //每次增加任务的个数
typedef struct 
{
    void *(*function);
    void *arg;
}threadpool_task_t;

struct threadpool_t
{
    //锁住整个结构体
    pthread_mutex_t lock;
    //用于使用忙线程数时的锁
    pthread_mutex_t thread_counter;
    //任务队列不为满的时候
    pthread_cond_t queue_not_full; 
    //任务队列不为空的时候
    pthread_cond_t queue_not_empty;  
    //存放线程的tid,实际上就是管理了线程数组    
    pthread_t *threads;
    //管理者线程tid
    pthread_t admin_tid;
    //任务队列 
    threadpool_task_t *task_queue;  

    //线程池信息
    int min_thr_num; //线程中最小线程数
    int max_thr_num;   //线程中最大线程数
    int live_thr_num;  //线程池中存放的线程数
    int busy_thr_num;  //线程忙，正在工作的线程
    int wait_exit_thr_num;   //需要销毁的线程数


    //任务队列信息
    int queue_front;   //队头
    int queue_rear;     //队尾
    int queue_size;

    //存放的任务数
    int queue_max_size;
    //线程池状态
    int shutdown;
};
//销毁线程池
int 
threadpool_destroy(struct threadpool_t *pool);

int 
threadpool_free(struct threadpool_t *pool);

int 
is_thread_alive(pthread_t tid);
//工作线程
void *
threadpool_thread(void *threadpool);

//创建线程池
struct threadpool_t *
threadpoop_create(int min_thr_num,int max_thr_num,int queue_max_size);

//向线程池的任务队列中添加一个任务
int
pthreadpool_add_task(struct threadpool_t *pool,void *(*function)(void *arg),void* arg);

//管理线程
void *
admin_thread(void *threadpool);

int
main(void)
{

    return 0;
}
struct threadpool_t *
threadpoop_create(int min_thr_num,int max_thr_num,int queue_max_size)
{
    int i;
    struct threadpool_t *pool = NULL;
    do
    {
        //线程池空间开辟
        if((pool =(struct threadpool_t*)malloc(sizeof(struct threadpool_t)))==NULL)
        {
            printf("malloc threadpool false.\n");
            break;
        }
        //初始化
        pool->min_thr_num = min_thr_num;
        pool->max_thr_num = max_thr_num;
        pool->busy_thr_num =0;
        pool->live_thr_num = min_thr_num;
        pool->wait_exit_thr_num =0;
        pool->queue_front = 0;
        pool->queue_rear = 0;
        pool->queue_size = 0;
        pool->queue_max_size = queue_max_size;
        pool->shutdown =false;
        pool->threads =(pthread_t *)malloc(sizeof(pthread_t)*max_thr_num);
        if(pool->threads == NULL)
        {
            printf("malloc threads false.\n");
            break;
        }
        memset(pool->threads,0,sizeof(pthread_t)*max_thr_num);

        //队列开空间
        pool->task_queue = (threadpool_task_t*)malloc(sizeof( threadpool_task_t)*queue_max_size);
        if(pool->task_queue ==NULL)
        {
            printf("malloc task queue false.\n");
            break;
        }
        //初始化锁和条件变量
        if(pthread_mutex_init(&pool->lock,NULL)!=0 || pthread_mutex_init(&pool->thread_counter,NULL)!=0 ||
        pthread_cond_init(&pool->queue_not_empty,NULL)!=0 || pthread_cond_init(&pool->queue_not_full,NULL)!=0)
        {
            printf("init lock or cond false\n");
            break;
        }
        //启动
        for(i = 0;i<min_thr_num;i++)
        {
            pthread_create(&pool->threads[i],NULL,threadpool_thread,(void *)pool);
            printf("start thread %d...\n",(unsigned int)pool->threads[i]);
        }
        //管理者线程
        pthread_create(&pool->admin_tid,NULL,admin_thread,(void *)pool);
       return pool;

    } while (0);
    //释放线程池空间
    threadpool_free(pool);
    return NULL;
    
    
}
void *
threadpool_thread(void *threadpool)
{
    struct threadpool_t *pool =(struct thread_t *)threadpool;
    threadpool_task_t task;
    while(true)
    {
        pthread_mutex_lock(&pool->lock);
        //无任务则阻塞在 “任务队列不为空”上,有任务则拆包
        // queue->size 代表的是存在的任务数
        while((pool->queue_size == 0) && (!pool->shutdown))  //false
        {
            printf("任务队列为空\n");
            printf("thread %d is waiting \n",(unsigned int)pthread_self());
            pthread_cond_wait(&(pool->queue_not_empty),&pool->lock);
            //判断是否需要清楚线程.自杀功能
            if(pool->wait_exit_thr_num > 0)
            {
                pool->wait_exit_thr_num--;
                if(pool->live_thr_num > pool->min_thr_num)
                {
                    printf("thread %d is exiting \n",(unsigned int)pthread_self());
                    pool->live_thr_num--;
                    pthread_mutex_unlock(&(pool->lock));
                    pthread_exit(NULL);   //直接结束线程
                }
            }
        }   
    
        //线程池开关状态
        if(pool->shutdown)   //关闭线程池
        {
            pthread_mutex_unlock(&pool->lock);
            printf("thread %d is exiting \n",(unsigned int)pthread_self());
            pthread_exit(NULL);    //这里就是关闭工作线程，也就是说如果pool->shutdown 被设置为 true 那么，所有的线程都会
            //推出
        }
        //该线程就去拿任务
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg =pool->task_queue[pool->queue_front].arg;

        pool->queue_front = (pool->queue_front+1)%pool->queue_max_size;
        pool->queue_size--;
        //每拿一个线程，就要发送任务队列不为满的信号
        //向所有的线程发送信号
        pthread_cond_broadcast(&pool->queue_not_full);

        //释放线程锁
        pthread_mutex_unlock(&(pool->lock));

        //执行刚才取出的任务
        printf("thread %d start working \n",(unsigned int)pthread_self());
        pthread_mutex_lock(&pool->thread_counter);   //锁住忙线程变量
        pool->busy_thr_num++;
        pthread_mutex_unlock(&pool->thread_counter);

        //执行任务
        (*(task.function))(task.arg);

        //任务结束处理
        printf("thread %d end working \n",(unsigned int)pthread_self());
        pthread_mutex_lock(&pool->thread_counter);
        pool->busy_thr_num--;
        pthread_mutex_unlock(&pool->thread_counter);
    }
    pthread_exit(NULL);
}

int
pthreadpool_add_task(struct threadpool_t *pool,void *(*function)(void *arg),void* arg)
{
    pthread_mutex_lock(&pool->lock);
    
        //如果队列满了，调用wait阻塞
    while((pool->queue_size == pool->queue_max_size)&& (!pool->shutdown))
    pthread_cond_wait(&pool->queue_not_full,&pool->lock);
        
        //如果线程池处于关闭状态
    if(pool->shutdown)
    {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }

        //清空工作线程的回调函数的参数arg
    if(pool->task_queue[pool->queue_rear].arg != NULL)
    {
        //free(pool->task_queue[pool->queue_rear].arg);
        pool->task_queue[pool->queue_rear].arg=NULL;
    }

        //添加任务到任务队列
    pool->task_queue[pool->queue_rear].function=function;
    pool->task_queue[pool->queue_rear].arg=arg;
    pool->queue_rear = (pool->queue_rear+1)%pool->queue_max_size;
    pool->queue_size++;
        
        //添加完任务后，队列不为空，唤醒线程池中的一个线程
    pthread_cond_signal(&pool->queue_not_empty);
    pthread_mutex_unlock(&pool->lock);
    return 0;
}

void *
admin_thread(void *threadpool)
{
    int i;
    struct threadpool_t *pool = (struct pthread_t *)threadpool;
    while(!pool->shutdown)
    {
        printf("admin ------------\n");
        sleep(DEFAULT_TIME);
        pthread_mutex_lock(&pool->lock);
        int queue_size  = pool->queue_size;
        int live_thr_num = pool->live_thr_num;
        pthread_mutex_unlock(&pool->lock);

        pthread_mutex_lock(&pool->thread_counter);
        int busy_thr_num = pool->busy_thr_num;   //忙线程数
        pthread_mutex_unlock(&pool->thread_counter); 

        printf("admin busy live -%d--%d-\n",busy_thr_num,live_thr_num);

        //创建新的线程 实际任务数量大于 最小正在等待的任务数量 ，存活线程数小于最大线程数
        if(queue_size >=MIN_WAIT_TASK_NUM && live_thr_num <= pool->max_thr_num)
        {
            printf("admin add--------\n");
            pthread_mutex_lock(&pool->lock);
            int add = 0;
            
            //一次增加
            for(i=0;i<pool->max_thr_num && add<DEFAULT_THREAD_NUM && pool->live_thr_num <pool->live_thr_num;i++)
            {
                if(pool->threads[i] == 0 || !is_thread_alive(pool->threads[i]))
                {
                    pthread_create(&pool->threads[i],NULL,threadpool_thread,(void*)pool);
                    add++;
                    pool->live_thr_num++;
                    printf("new thread ----------------\n");
                }
            }
            pthread_mutex_unlock(&pool->lock);
        }
        //销毁多余的线程 
        if((busy_thr_num*2)<live_thr_num && live_thr_num > pool->min_thr_num)
        {
            printf("admin busy --%d--%d----\n",busy_thr_num,live_thr_num);
            
        }
    }
}

int 
is_thread_alive(pthread_t tid)
{
    int kill_rc = pthread_kill(tid,0);  //发送0 号信号，测试是否存活
    if(kill_rc == ESRCH)  //线程不存在
    {
        return false;
    }
    return true;
}

int 
threadpool_free(struct threadpool_t *pool)
{
    if(pool == NULL)
    return -1;
    if(pool->task_queue)
    free(pool->task_queue);
    if(pool->threads)
    {
        free(pool->threads);
        pthread_mutex_lock(&pool->lock);
        pthread_mutex_destroy(&pool->lock);
        pthread_mutex_lock(&pool->thread_counter);
        pthread_mutex_destroy(&pool->thread_counter);
        pthread_cond_destroy(&pool->queue_not_empty);
        pthread_cond_destroy(&pool->queue_not_full);
    }
    free(pool);
    pool = NULL;
    return 0;
}
//销毁线程池
int 
threadpool_destroy(struct threadpool_t *pool)
{
    int i;
    if(pool == NULL)
    return -1;
    pool->shutdown = true;

    //销毁管理者线程
    pthread_join(pool->admin_tid,NULL);
    //通知所有线程去自杀(在自己领任务的过程中)
    for(i=0;i<pool->live_thr_num;i++)
    {
        pthread_cond_broadcast(&pool->queue_not_empty);
    }
    //等待线程结束 显示pthread_exit 然后等待其结束
    for(i=0;i<pool->live_thr_num;i++)
    {
        pthread_join(pool->threads[i],NULL);
    }
    threadpool_free(pool);
    return 0;
}