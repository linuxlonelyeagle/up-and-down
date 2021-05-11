#include<stdio.h>
#include<vector>

//类有两种写法，c和c++，类可以先被声明，然后在定义，声明之后可以有他的引用和指针，但是不能定义包含这个类的函数，
//但是可以声明这种函数。一个类不能包含自己，只有全部定义才有意义，但是可以有他的引用和指针包含在成员里面
using namespace std;
class X;
class Y;
class X
{
    Y*p;
};
class Y
{
    X a;
};
int
main()
{

    return 0;
}
