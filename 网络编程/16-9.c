#if defined(SOLARIS)
#include<netinet/in.h>
#endif
#include<netdb.h>
#include<arpa/inet.h>
#if defined(BSD)
#include<sys/socket.h>
#include<netinet.h>
#endif
#include<stdio.h>
#include<stdlib.h>

void 
print_family(struct addrinfo *aip)
{
    printf(" family ");
    switch(aip->ai_family)
    {
    case AF_INET:
        printf("inet");
        break;
    case AF_INET6:
        printf("inet6");
        break;
    case AF_UNIX:
        printf("unix");
        break;
    case AF_UNSPEC:
        printf("unspecified");
        break;
    default:
        printf("unknwon");
    }
}
void 
print_type(struct addrinfo *aip)
{
    printf(" type ");
    switch(aip->ai_socktype)
    {
    case SOCK_STREAM:
        printf("stream");
        break;
    case SOCK_DGRAM:
        printf("datagram");
        break;
    case SOCK_SEQPACKET:
        printf("seqpacket");
        break;
    case SOCK_RAW:
        printf("raw");
        break;
    default:
        printf("unknown (%d)",aip->ai_socktype);
    }
}

void 
print_protocol(struct addrinfo *aip)
{
    printf(" protocol ");
    switch(aip->ai_protocol)
    {
    case 0:
        printf("default");
        break;
    case IPPROTO_TCP:
        printf("TCP");
        break;
    case IPPROTO_UDP:
        printf("UDP");
        break;
    case IPPROTO_RAW:
        printf("raw");
        break;
    default:
        printf("unknown (%d)",aip->ai_protocol);
    }
}

void 
print_flags(struct addrinfo *aip)
{
    printf("flags");
    if(aip->ai_flags==0)
    {
        printf(" 0");
    }
    else
    {
        if(aip->ai_flags & AI_PASSIVE)
            printf(" passive");
        if(aip->ai_family & AI_CANONNAME)
            printf(" canon");
        if(aip->ai_flags & AI_NUMERICHOST)
            printf(" numhost");
        if(aip->ai_flags & AI_NUMERICSERV)
            printf(" numserv");
        if(aip->ai_flags & AI_V4MAPPED)
            printf(" v4mapped");
        if(aip->ai_flags & AI_ALL)
            printf(" all");
    }
}

int
main(int argc,char *argv[])
{
    struct addrinfo *ailist,*aip;
    struct addrinfo hint;
    struct sockaddr_in *sinp;
    const char *addr;
    int err;
    char abuf[INET_ADDRSTRLEN];

    if(argc != 3)
    {
        printf("usage: %s nodename service",argv[0]);
        exit(1);
    }
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = 0;
    
}
