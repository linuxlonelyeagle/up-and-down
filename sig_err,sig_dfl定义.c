#include<stdio.h>
#define SIG_ERR (void (*)(int))-1
#define SIG_DEL (void (*)(int))0
#define SIG_IGN (void (*)(int))1
//上面涉及到了强制类型转换
int
main(void)
{
    printf("SIG_ERR %d\n",SIG_ERR);  //输出-1   
    printf("SIG_IGN %d\n",SIG_IGN);  //输出1
    printf("SIG_DEL %d\n",SIG_DEL);   //0
    return 0;
}
