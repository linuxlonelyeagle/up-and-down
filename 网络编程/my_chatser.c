#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<sys/epoll.h>
#include<signal.h>
#include<fcntl.h>
#include"./cJSON.h"
#include<pthread.h>
#include<mysql/mysql.h>
#include<time.h>

#define  LISTENQ 1024
#define MAXEVENTS 64
#define buffersize   1024

//用户的唯一标识
long acount_number = 1000000;
pthread_mutex_t  acount_mutex;
pthread_cond_t acount_cond;

struct sign_up
{
    pthread_t pth;
    int connfd;
    char buf[buffersize];
}sign;
pthread_mutex_t sign_mutex ;
pthread_cond_t sign_cond;

struct find_back
{
    pthread_t pth;
    int connfd;
    char buf[buffersize];
}fin;
pthread_mutex_t find_mutex;
pthread_cond_t find_cond;


//错误处理函数
void
my_err(char *string,int line);

int 
open_listenfd(int port);

//自己封装的recv
void*
my_recv(void *arg);

//将文件描述符设置为非阻塞状态
int
setnonblocking(int fd);

int 
my_read(int connfd,char *buf1,char *buf2);

//解析输入
int 
find_function(int connfd,char *buf);

//注册用的函数
void*
sign_up(void*arg);
//找回密码
void*
find_back(void*arg);

int 
main(int argc,char*argv[])
{
    pthread_mutex_init(&sign_mutex,NULL);
    pthread_cond_init(&sign_cond,NULL);
    
    pthread_mutex_init(&find_mutex,NULL);
    pthread_cond_init(&find_cond,NULL);

    pthread_mutex_init(&acount_mutex,NULL);
    pthread_cond_init(&acount_cond,NULL);

    signal(SIGPIPE,SIG_IGN);
    char buf1[buffersize];
    char buf2[buffersize];
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    int port = atoi(argv[1]);
    int listenfd = open_listenfd(port);
    printf("%d\n",listenfd);
    int efd;
    if((efd=epoll_create(1))==-1)
        my_err("epoll_create",__LINE__);
    struct epoll_event event;
    struct epoll_event *events=NULL;
    event.data.fd = listenfd;
    event.events = EPOLLIN;   
    if(epoll_ctl(efd,EPOLL_CTL_ADD,listenfd,&event)==-1)
    my_err("epoll_ctl",__LINE__);
    events = (struct epoll_event*)calloc(MAXEVENTS,sizeof(struct epoll_event));
    if(events==NULL)
        my_err("calloc",__LINE__);
    int connfd;
    while(1)
    {
        int n,i;
        n = epoll_wait(efd,events,MAXEVENTS,-1);
        for(i = 0;i<n;i++)
        {
            //if(events[i].events &EPOLLERR || events[i].events &EPOLLHUP)
           // {
            //    printf("关闭文件描述符%d",events[i].data.fd);
             //   epoll_ctl(efd,EPOLL_CTL_DEL,events[i].data.fd,&event);
             //   close(events[i].data.fd);
            //}
             if(events[i].data.fd == listenfd)
            {
              connfd = accept(listenfd,(struct sockaddr*)&addr,&len);
              if(connfd<0)
                    my_err("accept",__LINE__);
              event.data.fd = connfd;
              event.events = EPOLLIN|EPOLLET;
              setnonblocking(connfd);
              epoll_ctl(efd,EPOLL_CTL_ADD,connfd,&event);
            }
            else if (events[i].events&EPOLLIN)
            {
                memset(buf2,0,strlen(buf2));
                recv(events[i].data.fd,buf2,buffersize,0);
                find_function(events[i].data.fd,buf2);
            }
            
        }
    }
    close(efd);
    return 0;
}

void *
find_back(void *arg)
{
    struct find_back a=fin;
    pthread_cond_broadcast(&find_cond);
    //连接数据库查询东西
    MYSQL mysql;
    if(NULL==mysql_init(&mysql))
        my_err("mysql_init",__LINE__);
    if(mysql_library_init(0,NULL,NULL)!=0)
        my_err("mysql_library_init",__LINE__);
    if(NULL == mysql_real_connect(&mysql,"127.0.0.1","root","123456","my_chat",0,NULL,0))
        my_err("mysql_real_connect",__LINE__);
    if(mysql_set_character_set(&mysql,"utf8")<0)
        my_err("mysql_set_character_set",__LINE__);

    char cmd[256];
    char question[256];
    char answer[256];
    char password[256];
    char buf[256];
    cJSON *root =NULL;
    cJSON *item = NULL;
    root = cJSON_Parse(a.buf);
    item = cJSON_GetObjectItem(root,"find_back");
    sprintf(cmd,"select *from user_data where account = %s",item->valuestring);
    if(mysql_query(&mysql,cmd)!=0)
        my_err("mysql_query",__LINE__);
    MYSQL_RES *res = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    sprintf(question,"%s",row[3]);
    sprintf(answer,"%s",row[4]);
    sprintf(password,"%s",row[2]);

    send(a.connfd,question,256,0);
    recv(a.connfd,buf,256,0);
    if(strcmp(buf,answer)==0)
        send(a.connfd,password,256,0);
    else
    {
        strcpy(buf,"答案错误");
        send(a.connfd,password,256,0);
    }
    return NULL;
}

void*
sign_up(void *arg)
{
    
    struct sign_up si = *(struct sign_up*)arg;
    pthread_cond_broadcast(&sign_cond);
    pthread_detach(pthread_self());
    pthread_detach(si.pth);
    char name[30];
    char password[30];
    char question[256];
    char answer[256];
    char acountnumber[20];
    cJSON * root = NULL;
    cJSON *item = NULL;
    cJSON *next = NULL;
    root = cJSON_Parse(si.buf);
    item = cJSON_GetObjectItem(root,"sign_up");
    next = cJSON_GetObjectItem(item,"name");
    sprintf(name,"%s",next->valuestring);
    next = cJSON_GetObjectItem(item,"password");
    sprintf(password,"%s",next->valuestring);
    next = cJSON_GetObjectItem(item,"question");
    sprintf(question,"%s",next->valuestring);
    next = cJSON_GetObjectItem(item,"answer");
    sprintf(answer,"%s",next->valuestring);
    sprintf(acountnumber,"%d",acount_number);
    pthread_mutex_lock(&acount_mutex);
    send(si.connfd,acountnumber,30,0);
    acount_number++;
    pthread_mutex_unlock(&acount_mutex);
    
    //连接数据库
    MYSQL mysql;
    if(mysql_init(&mysql)==NULL)
            my_err("mysql_init",__LINE__);
    //初始化数据库
    if(mysql_library_init(0,NULL,NULL)!=0)
        my_err("mysql_library_init",__LINE__);
    //连接数据库
    if(mysql_real_connect(&mysql,"127.0.0.1","root","123456","my_chat",0,NULL,0)==NULL)
        my_err("mysql_library_init",__LINE__);
    //设置中文字符
    mysql_set_character_set(&mysql,"utf8");
    char cmd[256];
    printf("%s\n",answer);
    sprintf(cmd,"insert user_data values(\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",NULL,NULL)",acountnumber,name,password,question,answer);
    int a= mysql_query(&mysql,cmd);
    if(!a)
        printf("注册成功\n");
    else 
        printf("注册失败\n");
    mysql_close(&mysql);
    mysql_library_end();
    return NULL;
}


int
find_function(int connfd,char *str)
{
   char buf[30];
   int j = 0;
   char *find = strchr(str,':');
    for(char * i = str+2;i!=find;i++)
    {
       if(*i!='\"')
       {    
           buf[j]=*i;
           j++;
        }
    }
    buf[j]='\0';
    if(strcmp(buf,"sign_up")==0)
    {
        
        sign.connfd=connfd;
        strcpy(sign.buf,str);
        pthread_create(&sign.pth,NULL,sign_up,(void*)&sign);
        pthread_mutex_lock(&sign_mutex);
        pthread_cond_wait(&sign_cond,&sign_mutex);
        pthread_mutex_unlock(&sign_mutex);
    }
    else if(strcmp(buf,"find_back")==0)
    {
        fin.connfd = connfd;
        strcpy(fin.buf,str);
        pthread_create(&fin.pth,NULL,find_back,NULL);
        pthread_create(&fin.pth,NULL,find_back,NULL);
        pthread_mutex_lock(&find_mutex);
        pthread_cond_wait(&find_cond,&find_mutex);
        pthread_mutex_unlock(&find_mutex);
    }
   return 0;
}

int 
my_read(int connfd,char *buf1,char *buf2)
{
    int sum = 0;
    while(1)
    {
        int n;
        n = read(connfd,buf1,buffersize);
        if(n==0)
            return 0;
        else if(n==-1)
            return 1;
        sum +=n;
        memcpy(buf2,buf1,n);
        char *find = strchr(buf2,'\n');
        if(find !=NULL)
            *find = '\0';
    }
    return 0;
    
}

void
my_err(char *string,int line)
{
    printf("%d %s error\n",line,string);
    printf("%s\n",strerror(errno));
    exit(1);
}

int
open_listenfd(int port)
{
     int listenfd,optval=1;
     struct sockaddr_in serveraddr;
     if((listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
     my_err("socket",__LINE__);

     if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int))<0)
     my_err("setsockoptk",__LINE__);
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons((unsigned short)port);
       if (bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
       my_err("bind",__LINE__);
        if (listen(listenfd, LISTENQ) < 0)
            my_err("listenfd",__LINE__);
        return listenfd;

 }

int 
setnonblocking(int fd)
{
    int old_option  = fcntl(fd,F_GETFL);
    int new_option = old_option|SOCK_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

