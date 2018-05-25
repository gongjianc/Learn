#include <iostream>

using namespace std;

int main()
{
    const int *p1 = new int; //自动类型转换
    cout << "p1 is " << *p1 << endl;  //undefined 并代表初始值不是0
    int *p2 = new int();
    cout << "p2 is " << *p2 << endl;
    int *p3 = new int(3);
    cout << "p3 is " << *p3 << endl;
}
