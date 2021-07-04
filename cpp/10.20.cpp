#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>

using namespace std;

int
main(void)
{
    int count;
    auto f=[](string str)
    {
        return str.size()>=6;
    };
    vector<string> strs={"fox","red","the","over","slow","jumps","quick","turtle"};
    count=count_if(strs.begin(),strs.end(),f);
    cout<<count<<endl;
    return 0;
}