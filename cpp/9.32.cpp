#include<iostream>
#include<vector>
using namespace std;

int 
main(void)
{
    vector<int> vi={0,1,2,3,4,5,6,7,8,9};
    auto iter =vi.begin();
    while(iter != vi.end())
    {
        if(*iter % 2)
        {
            iter = vi.insert(iter,(*iter)++);  //不能写成*iter++，不然的花这个iter迭代器会自加的
            iter+=2;
        }
        else
        iter = vi.erase(iter);
    }
    for(auto i:vi)
    {
        cout<<i<<endl;
    }
    return 0;
}