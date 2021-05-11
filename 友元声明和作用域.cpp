#include<iostream>
#include<vector>
using namespace std;

class a
{
public:
    a()=default;
    a(int d,int e):b(d),c(e){};
    friend void print(a);
    void print()
    {
        cout<<b<<" "<<c<<endl;
    }
private:
    int b;
    int c;
};
void print(a c)
{
    cout<<c.b<<" "<<c.c<<endl;
}
int
main(void)
{

    return 0;
}
