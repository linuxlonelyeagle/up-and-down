#include<stdio.h>

int 
main(void)
{
    int a=1;
    int *p=&a;
    printf("%d\n",p);
    int b=*p++;   //这个自加到指针上面去了
    printf("%d %d\n",b,p);
    int c=1;
    p=&c;
    printf("%d\n",p);
    int d=++*p;
    printf("%d %d",p,d);
    return 0;
}