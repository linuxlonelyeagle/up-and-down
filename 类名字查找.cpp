#include<iostream>
#include<vector>
using namespace std;
int b;

class a
{
public:
    friend void print(){
        cout<<b<<endl;           //先在定义这个函数的块里面查找,然后再在外面找，也就是这个class,  
    }           //如果private 里面是b 的话，报错，如果class里面没有b就取外面找,这里的话就是全局的b
private:            //原因就是，会先处理完声明，在处理定义，也就是说private里面的b是可以看到的，所以能够使用所以的名字,
    int a;           //当类里面使用声明的时候，必须要是可以看到的，声明跟定义是不一样的
};
void print();
int
main(void)
{
    print();   //输出0 ;
    return 0;
}
