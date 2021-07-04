#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int
main(void)
{
    char buffer[32];
    int ret= 0;
    int host = 0;
    int network = 0;
    unsigned int address = 0;
    struct in_addr in;

    in.s_addr =0;
    printf("please input your ip address:");
    fgets(buffer,31,stdin);
    buffer[31]='\0';

    //事例使用inet_aton函数
    if((ret = inet_aton(buffer,&in))==0)
    {
        printf("inet_aton:\t invalid address\n");
    }
    else
    {
        printf("inet_aton:\t%u\n",in.s_addr);
    }

    //事例使用inet_addr函数 与inet_aton函数类似，转换成网络序
    if((address = inet_addr(buffer))==INADDR_NONE)
    {
        printf("inet_addr: \t invalid address\n");
    }
    else
    {
        printf("inet_addr:\t%u\n",address);
    }
    //事例使用inet_network函数 吧网络字节序转换成主机字节顺序
    if((address = inet_network(buffer))==-1)
    {
        printf("inet_network:\t invalid address\n");
    }
    else{
        printf("inet_network:\t%u\n",address);
    }

    //事例使用inet_ntoa函数
    if(inet_ntoa(in)==NULL){
    printf("inet_ntoa: \t invalid address\n");
    }else{
    printf("inet_ntoa:\t %u\n",inet_ntoa(in));
    }

    //实例使用inet_lnaof() 与 inet_netof
    host = inet_lnaof(in);
    network = inet_netof(in);

    printf("inet_lnaof:\t %u\n",host);
    printf("inet_netof:\t %u\n",network);
    in  =  inet_makeaddr(network,host);
    printf("inet_makeaddr:%u\n",in.s_addr);
    return 0;

}
