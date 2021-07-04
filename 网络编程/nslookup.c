#include"csapp.h"
int
main(int argc,char **argv)
{
    struct addrinfo *p,*listp,hists;
    char buf[MAXLINE];
    int rc,flags;

    if(argc!=2){
        fprintf(stderr,"usage: %s <domain name>\n",argv[0]);
        exit(0);
    }
    //get a list of addrinfo records
    
    memset(&hists,0,sizeof(struct addrinfo));
    hists.ai_family = AF_INET;
    hists.ai_socktype = SOCK_STREAM;
    if((rc = getaddrinfo(argv[1],NULL,&hists,&listp))!=0)
    {
        fprintf(stderr,"getaddrinfo error:%s\n",gai_strerror(rc));
        exit(1);
    }

    flags = NI_NUMERICSERV;
    for(p = listp;p;p=p->ai_next)
    {
        getnameinfo(p->ai_addr,p->ai_addrlen,buf,MAXLINE,NULL,0,flags);
        printf("%s\n",buf);
    }
    freeaddrinfo(listp);
    return 0;
}
