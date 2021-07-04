#include<iostream>
#include<list>
using namespace std;
int
main(void)
{
    list<int> lit;
    for(int i=0;i<10;i++)
    {
        lit.push_back(i);
    }
    auto iter = lit.begin();
    auto a=lit.erase(iter);
    cout<<*a<<endl;   //输出最后原来的元素的后面一个元素
    auto iter1=lit.end();
    auto b=lit.erase(iter1);
    cout<<*b<<endl;   //错误
    return 0;
}