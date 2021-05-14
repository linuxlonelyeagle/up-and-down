#include<iostream>
using namespace std;

int
main(void)
{
    //tie 有参数的时候也会返回关联的流的指针
    cin.tie(&cout);
    ostream * p=cin.tie(nullptr);
    *p<<"hello world"<<endl;

    return 0;
}
