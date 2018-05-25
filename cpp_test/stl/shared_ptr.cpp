#include <memory>
#include <iostream>
#include <string>

using namespace std;

template <typename T>
void print(const shared_ptr<T> &t)
{
    cout << "The res is :" << t << endl;
}

int main()
{
    string tmp("hello,world");
    shared_ptr<string> p1;
    if (p1 && p1->empty()){
        *p1 = "Hello, world";
    }
    print(p1);
    shared_ptr<string> p2 = make_shared<string>(10, '9');
    print(p2);
}
