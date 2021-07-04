#include<stdio.h>
using namespace std;

int
main(void)
{

    int a = 10;    
    printf("%d\n",&a);
    auto f = [a](){printf("%d\n",&a);};
    f();    
    //上面打印的两次的结果是不一样的,说明匿名函数的捕获是一个值的拷贝
    return 0;
}