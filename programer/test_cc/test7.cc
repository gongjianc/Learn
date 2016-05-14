/*
** @file    test7.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-04-28 16:21:31
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

class B{
public:
    B() : b(1) {}
    //virtual void foo() {}
    int b;
};

class D : public B{
public:
    D() : d(2) {}
    int d;
};

void func(B *pb)
{
    D *pd1 = static_cast<D *>(pb);
    cout << pd1->b << endl;
    cout << pd1->d << endl;
    D *pd2 = dynamic_cast<D *>(pb);
    cout << pd2->b << endl;
    cout << pd2->d << endl;
}

int main(void)
{
    B *pb = new B;
    func(pb);
    return 0;
}


