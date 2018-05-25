#include <string>
#include <memory>
#include <iostream>
#include <cstddef>
using namespace std;

void process(shared_ptr<int> ptr)
{
    cout << "process: ptr is " << *ptr << endl;
    cout << "process: ptr's count  is " << ptr.use_count() << endl;
}

int main()
{
    unique_ptr<string> p1(new string("hello"));
    /* unique_ptr<string> p2(p1); */
    unique_ptr<string> p2(p1.release());
    cout << "p2 is " << *p2 << endl;
    unique_ptr<string> p3(new string("world"));
    p2.reset(p3.release());
    cout << "p2 is " << *p2 << endl;
    /* p2.reset(nullptr_t); */
    p2.reset();
    /* cout << "p2 is " << endl << *p2 << endl; */
    //release meber returns the pointer currently stored in the unique_ptr and makes the unique_ptr null <-- NULL
    /* cout << "p3 is " << *p3 << endl; */

    int *p4 = new int(8);
    shared_ptr<int> p5(p4);
    process(p5);
    process(shared_ptr<int>(p4));
    cout << "p4 is " << *p4 << endl;
    cout << "p5 is " << *p5 << endl;
    cout << "p5's use_count is " << p5.use_count() << endl;

    int num = 8; //栈空间
    int *p6 = &num;
    shared_ptr<int> p7(p6); // where shared_ptr allocate heap or stack?? 
    cout << "p7 is " << *p7 << endl;
}
