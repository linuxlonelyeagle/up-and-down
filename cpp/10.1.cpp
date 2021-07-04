#include<algorithm>
#include<iostream>
#include<vector>
#include<numeric>
using namespace std;

int
main(void)
{
    vector<double> a={1.1,2.2,3.3,4.4,5.5,6.,6};
    auto n=accumulate(a.cbegin(),a.cend(),0.0);
    cout<<n<<endl;
    return 0;
}