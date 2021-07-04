#include<iostream>
#include<forward_list>
using namespace std;

int 
main(void)
{
    forward_list<int> a={1,2,3,4,5,6,7,8,9,10};
    auto bef = a.before_begin();
    auto cur = a.begin();
    while(cur!=a.end())
    {
        if(*cur%2)
        {
            cur=a.erase_after(bef);   //这种情况的花cur始终都是在bef始终都是在cur前面的
        }
        else if (*cur%2==0)
        {
            bef=cur;
            cur++;
        }
    }
    for(auto i:a)
    {
        cout<<i<<endl;
    }
    return 0;
}