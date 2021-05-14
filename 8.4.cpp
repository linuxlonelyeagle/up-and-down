#include<iostream>
#include<vector>
#include<fstream>
using namespace std;
int
f(vector<string> &strs,string argv)
{
    ifstream input(argv);
    if(!input)
    return -1;
    string pt;
    while(getline(input,pt))
    {
        strs.push_back(pt);
    }
    return 0;
}
void 
print(vector<string> &strs)
{
    ofstream print("out.txt");
    for(auto str:strs)
    {
        print<<str<<endl;
    }
}

int
main(int argc,char *argv[])
{
    vector<string> strs;
    int a=f(strs,argv[1]);
    if(a!=0)
        printf("打开文件错误");
    for(auto str:strs)
    {
        cout<<str<<endl;
    }
    return 0;
}
