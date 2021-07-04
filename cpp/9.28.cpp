#include<iostream>
#include<forward_list>
using namespace std;
void 
f(forward_list<string>& strs,string a,string b)
{
    int flag = 0;
    auto j = strs.begin();
    j++;
    for(auto i = strs.begin();i!=strs.end();i++)
    {
        
        if(*i==a)
        {
            *i=*i+b;
            break;
        }
        else if(j==strs.end())
        {
            strs.insert_after(i,b);
            break;  //这里必须要结束，不然的花程序就要崩溃
        }
        j++;
    }


}
int
main(void)
{
    forward_list<string> strs={"aaaaaa","world"};
    f(strs,"helho","world14");
    for(auto i:strs)
    {
        cout<<i<<endl;
    }
    return 0;
}