#include<iostream>
#include<vector>
using namespace std;
string str;
void 
read(std::istream&is)
{
    is>>str;
}
void
print(std::ostream&os)
{
    os<<str<<endl;
}
int 
main(void)
{
    read(cin);
    print(cout);
    return 0;
}
