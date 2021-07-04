#include<iostream>
#include<vector>
#include<list>
using namespace std;
int
main(void)
{
    int ia[]={0,1,1,2,3,5,8,13,21,55,89};
    vector<int> a(begin(ia),end(ia));
    for(auto i=a.begin();i!=a.end();i++)
    {
        if(*i%2==0)
        a.erase(i);
    }
    cout<<"删除后的vector"<<endl;
    for(auto i:a)
    cout<<i<<" ";
    cout<<endl;
    list<int> b;
    for(auto i=begin(ia);i!=end(ia);i++)
    {
        if(*i%2==0)
        b.push_back(*i);
    }
    cout<<"删除之后的list"<<endl;
    for(auto i:b)
    cout<<i<<" ";
    cout<<endl;
    return 0;
}