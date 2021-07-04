#include<iostream>
#include<vector>
using namespace std;

int 
main(void)
{
    vector<string> strs;
    string str;
    while(cin>>str)
    strs.push_back(str);
    float sum=0;
    for(auto i:strs)
    {
        sum=sum+stof(i);
    }
    cout<<sum<<endl;
    return 0;
}