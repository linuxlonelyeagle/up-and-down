#include<iostream>
#include<algorithm>
#include<numeric>
#include<vector>
using namespace std;

int
main(void)
{
    vector<int> a;
    fill_n(back_inserter(a),10,0);
    replace_copy(a.begin(),a.end(),back_inserter(a),0,1);
    for(auto i:a)
    {
        cout<<i<<endl;
    }
    return 0;
}