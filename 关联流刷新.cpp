#include<iostream>
using namespace std;
int
main(void)
{
    cin.tie(&cout);
    cout<<"hello world"<<endl;   //hello world 这里是可以打印的
    string str;        //因为当一个输入流关联到一个输出流时候，任何试图从输入流读取数据的操作都会刷新关联到的输出流
    cin>>str;
    while(1)
        ;
    return 0;
}
