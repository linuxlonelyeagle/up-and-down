//编写程序,从vector<char> 初始化一个string

#include<iostream>
#include<vector>
using namespace std;

int
main(void)
{
    vector<char> p={'h','e','l','l','o',' ','w','o','r','l','d'};
    string str(p.begin(),p.end());
    cout<<str<<endl;
    return 0;
}