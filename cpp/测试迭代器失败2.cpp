#include<iostream>
#include<deque>
#include<stdio.h>
using namespace std;

int 
main(void)
{
    
    int count=0;
    deque<int> a{1,2,3,4,5,6,7};
    for(auto i=a.begin();i!=a.end();i++)
    {
        auto j = a.end();
        if(*i%2)
        {
           i=a.insert(i,*i);
            i++;
        }
        else
        {
            i=a.erase(i);
            i--;
        }
        if(j!=a.end())   //迭代器失效
        {
            printf("count:%d\n",count++);           
            printf("原来的迭代器:%d\n",j);
            printf("现在的迭代器:%d\n",a.end());            
        }
    }
    return 0;
}