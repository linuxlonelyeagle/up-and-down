#include<vector>
#include<iostream>
using namespace std;
string 
replace(string s,string oldval,string newval)
{
    string str;
    if(s.size()<oldval.size())
    {
        cout<<"原来的string太小"<<endl;
        return str;
    }
    int i=oldval.size();
    int j=0;
    while(i+j!=newval.size())
    {
        string tmp(s.begin()+j,s.begin()+i+j);
        if(tmp==oldval)
        {
            s.replace(s.begin()+j,s.begin()+i+j,newval);
            return s;
        }
        j++;
    }
    cout<<"no"<<endl;
    return str;
    
}
int
main(void)
{
    string  s("hello world");
    string oldval("hello");
    string newval("helloworld");
    string m=replace(s,oldval,newval);
    cout<<m<<endl;
    return 0;
}