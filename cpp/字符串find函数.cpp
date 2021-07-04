#include<iostream>
using namespace std;
int 
main(void)
{
    string str("hello world");
    int i = str.find("a");    
    cout<<i<<endl;   //输出-1
    unsigned int j =str.find("a");  
    cout<<j<<endl;  //输出无符号最大的那个数

    return 0;
}