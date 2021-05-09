#include<iostream>
#include<vector>
#include<cassert>
#include<cstring> //c++版本
using std::begin;
using std::end;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
class person
{
public:
    person(string name,string addr):name(name),addr(addr){}
    person()=default;
    person& copy()
    {
        return *this;
    }
    void print()
    {
        cout<<"name:"<<name<<endl;
        cout<<"addr:"<<addr<<endl;
    }
private:
    string name;
    string addr;
};
int 
main(int argc,char *argv[])
{
    person a("lonelyeagle","xiyou");
    person b=a.copy();
    b.print();
    return 0;
}
