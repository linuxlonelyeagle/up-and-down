#include<iostream>
#include<vector>
using namespace std;
istream &
f(istream &is)
{
    string str;
    while(is>>str)
    {
        cout<<str<<endl;
    }
    return is;
}

int
main(void)
{
    f(cin).clear();
    return 0;
}
