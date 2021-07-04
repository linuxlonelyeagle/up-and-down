#include<iostream>
#include<vector>
#include<list>
using namespace std;
int
main(void)
{
    list<string> strs;
    auto iter = strs.begin();
    string str;
    while(cin>>str)
    {
        iter = strs.insert(iter,str);
    }
    for(auto str:strs)
    {
        cout<<str<<endl;
    }
    return 0;
}