#include<iostream>
#include<list>
#include<vector>
#include<deque>
using namespace std;
int
main(void)
{
    vector<int> a={1,2,3,4,5};
    auto iter = a.end();
    for(auto i=a.begin();i!=a.end();i++)
    {

        if(*i%2==0)
        {
           i=a.insert(i,*i);
           i++;
        }
        else
        {
            i=a.erase(i);
            i--;
        }
    }
    for(auto i:a)
    {
        cout<<i<<endl;
    }

    return 0;
}