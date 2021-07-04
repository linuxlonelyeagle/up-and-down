#include<iostream>
#include<list>
#include<stdio.h>
using namespace std;
int
main(void)
{
    list<string> strs;
    string str;
    auto iter =strs.begin();
    while(cin>>str)
    {
        iter  = strs.insert(iter,str);
    }
    cout<<strs.size()<<endl;
    for(auto i= strs.end();i!=strs.begin();i--)
    {
        cout<<*i<<endl;
    }
    for(auto i=strs.rbegin();i!=strs.rend();i--)
    {
        cout<<*i<<endl;
    }
    return 0;
}
