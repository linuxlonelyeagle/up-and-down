#include<iostream>
#include<vector>

using namespace std;
int
main(void)
{
    string str("hello world");
    str.assign(str);
    cout<<str<<endl;
    return 0;
}