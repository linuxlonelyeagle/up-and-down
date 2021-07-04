#include<iostream>
using namespace std;

int 
main(void)
{
    int a,b;  
    cin>>a>>b;
    auto f=[](int a,int b){return a+b;};
    cout<<f(a,b)<<endl; 
    return 0;
}