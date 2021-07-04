#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<errno.h>
#include<signal.h>

#include"./cJSON.h"
//找回密码
void
find_back(int connfd);

void 
my_err(char *string,int line)
{
    printf("%d %s error!\n",line,string);
    printf("%s\n",strerror(errno));
    exit(1);
}
//连接
int
open_clientfd(char *hostname,int port)
{
    int clientfd;
    struct hostent *hp =NULL;
    struct sockaddr_in serveraddr;
    if((clientfd = socket(AF_INET,SOCK_STREAM,0))<0)
        my_err("socket",__LINE__);
    if((hp = gethostbyname(hostname))==NULL)
        my_err("gethostbyname",__LINE__);
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char*)hp->h_addr_list[0],(char*)&serveraddr.sin_addr,hp->h_length);
    serveraddr.sin_port = htons(port);
    
    //connect
    if(connect(clientfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
        my_err("connect",__LINE__);
    return clientfd;

}
void
start()
{
    printf("A.登陆、\n");
    printf("B.注册、\n");
    printf("C.找回密码、\n");
    printf("D.注销帐号、\n");
}

int
sign_up(int fd)
{
    char buf[1024];
    char name[30];
    
   printf("请输入你的用户名:");
   scanf("%s",name);
   while(getchar()!='\n');
   char passwd1[20];
   char passwd2[20];
   char question[256];
   char answer[256];
   while(1)
   {
        printf("请输入你的密码:");
        scanf("%s",passwd1);
        printf("请再次输入你的密码:");
        scanf("%s",passwd2);
        if(strcmp(passwd1,passwd2)==0)
        {
            printf("请输入你的密保问题:");
            scanf("%s",question);
            printf("请输入你的密保答案:");
            scanf("%s",answer);
            sprintf(buf,"{\"sign_up\":{\"name\":\"%s\",\"password\":\"%s\",\"question\":\"%s\",\"answer\":\"%s\"}}",name,passwd1,question,answer);
            if(send(fd,buf,156,0)<0)
            my_err("send",__LINE__);
            if(recv(fd,buf,156,0)<0)
                my_err("recv",__LINE__);
            printf("你的帐号是:%s\n",buf);
            while(getchar()!='\n');
            return 0;
        }
        else 
        {
            printf("两次输入的密码不一样，请重新输入!\n");
        }
   }
}
int
main(int argc,char *argv[])
{
 
    //信号处理
    signal(SIGPIPE,SIG_IGN);
    char op;
    int port = atoi(argv[2]);
    int connfd = open_clientfd(argv[1],port);
    start();
    while(1)
    {
        scanf("%c",&op);
        while(getchar()!='\n');
        switch(op)
        {
            case 'a':
                break;
            case 'b':
                sign_up(connfd);
                break;
            case 'c':   //找回密码
                find_back(connfd);
                break;     
            case 'd':     //注销帐号
                break;   
            default:
                 printf("输入错误，请重新输入.\n");
                 break;
        }

    }
    return 0;
}

//找回密码
void find_back(int connfd)
{
    char buf[256];
    char acount[20];
    printf("请输入你的帐号:");
    scanf("%s",acount);
    printf("%s",acount);
    sprintf(buf,"{\"find_back\":\"%s\"}",acount);
    send(connfd,buf,256,0);
    recv(connfd,buf,256,0);
    printf("你的密保问题是:%s\n",buf);
    printf("请输入答案:");
    scanf("%s",buf);
    send(connfd,buf,256,0);
    recv(connfd,buf,256,0);
    printf("%s\n",buf);
}
