#include<iostream>
using namespace std;

int 
main(void)
{
    int a=520,b=1314;
    auto f = [&](){return a=a+b;};   //也就是说引用全部外面的东西
    int c=f();
    cout<<a<<endl;
    a=520;
     auto n = [=]()mutable{return a=a+b;};   //这个a是只读变量
     c=n();
     cout<<a<<endl;   //a的值并没有被改变
     
    return 0;
}