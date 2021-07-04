#include"csapp.h"

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio,connfd);
    /*
     *  rp->rio_fd = fd;
     *  rp->rio_cnt = 0;
     *  rp->rio_bufptr = rp->rio_buf;
     */
    while((n = Rio_readlineb(&rio,buf,MAXLINE))!=0)
    {
        printf("server received %d bytes\n",(int)n);
        Rio_writen(connfd,buf,n);
        printf("%s\n",buf);
        //向  套接字写消息
    }
}

int
main(int argc,char *argv[])
{
    int listenfd,connfd;
    socklen_t clientlen;
    struct sockaddr_storage  clientaddr;
    char client_hostname[MAXLINE],client_port[MAXLINE];

    if(argc!=2)
    {
        fprintf(stderr,"usage:%s <port> \n",argv[0]);
        exit(0);
    }
    listenfd = atoi(argv[1]);
    listenfd = open_listenfd(listenfd);

    while(1)
    {
        clientlen  =  sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd,(SA*)&clientaddr,&clientlen);
        getnameinfo((SA*)&clientaddr,clientlen,client_hostname,MAXLINE,client_port,MAXLINE,0);
        //getnameinfo 函数取出主机名和服务器字符串
        printf("Connected to (%s %s)\n",client_hostname,client_port);
        echo(connfd);
        Close(connfd);
        printf("close connect\n");

    }
    exit(0);
}
