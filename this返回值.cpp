#include<iostream>
#include<vector>
using namespace std;
class person
{
public:
    person()=default;
    //如果这里不加&，如果返回值不是引用的话，那么就不会改变name和addr
   /* person &init(string nam ,string add)
    {
        name+=nam ;
        addr+=add;
        return *this;
    }
    */
    void init(string nam,string add)   //但是采用这样的形式是可以改变name和addr的，也就是说改变的不是副本,也就是说改变的东西跟前面的返回值是有关的
    {
        name+=nam;
        addr+=add;
    }
    void print()
    {
        cout<<name << " "<<addr<<endl;
    }
private:
    string name;
    string addr;
};
int
main(void)
{
     person a;
    a.init("yang","xiyou");
    a.print();  //如果返回的不是引用的花，就打印不出名字和地址了
    return 0;
}
