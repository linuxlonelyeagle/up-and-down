#include<vector>
#include<list>
#include<iostream>
using namespace std;
int
main(void)
{
   vector<int> a;
   for(int i=0;i<10;i++)
       a.push_back(i);
    auto b=a.back();
    cout<<b<<endl;
    return 0;
}