#include"csapp.h"


int
main(int argc,char *argv[])
{
    int clientfd;
    char *host,*port,buf[MAXLINE];
    rio_t rio;
    /*
     struct 
     {
     int rio_fd;
     int rio_cnt;
     char *buf_bufptr;
     char rio_buf[RIO_BUFSIZE];
     }
     */

    if(argc!=3)
    {
        fprintf(stderr,"usage:%s<host> <port>\n",argv[0]);
        exit(0);
    }

    host = argv[1];
    port = argv[2];
     //建立链接
     int portint = atoi(port);
    clientfd = open_clientfd(host,portint);

    Rio_readinitb(&rio,clientfd);
    /*
     * {
     *  rp->rio_fd = fd;
     *  rp->rio_cnt = 0;
     *  rp->rio_bufptr = rp->rio_buf;
     * }
     */

    while(Fgets(buf,MAXLINE,stdin)!=NULL)
    {
        //发送消息给已经连接的套接字   用write函数实现
        // 封装的函数用循环用strken(buf)保证所有的内容发送到
        // 服务器
        Rio_writen(clientfd,buf,strlen(buf));
        //接收消息 read函数实现
        Rio_readlineb(&rio,buf,MAXLINE);
        Fputs(buf,stdout);
    }
    Close(clientfd);
    return 0;
}
