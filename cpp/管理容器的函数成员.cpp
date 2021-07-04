#include<iostream>
#include<vector>
using namespace std;
int
main(void)
{
    vector<int> a;
    cout<<"size:"<<a.size()<<endl;  //0
    cout<<"capacity:"<<a.capacity()<<endl;   //0 
    a.reserve(10);
    cout<<"capacity"<<a.capacity()<<endl;   //10  这里因该大于等于10
    int i=0;
    while(a.size()!=a.capacity())
    {

        a.push_back(i++);
    }
    cout<<"size"<<a.size()<<endl;//10
    cout<<"capacity"<<a.capacity()<<endl;//10
    a.push_back(i);
    cout<<"size"<<a.size()<<endl;   //11
    cout<<"capacity"<<a.capacity()<<endl;   //20
    a.shrink_to_fit();
    cout<<"capacity"<<a.capacity()<<endl;   //11 .也就是说吧没有用到的容词的容量退回去了
    return 0;
}