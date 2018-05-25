#include <iostream>
#include <memory>

using namespace std;

typedef shared_ptr<int> pInt;

int main()
{
    /* shared_ptr<int> p(new int(42)); */
    auto p =pInt();
    /* int *q = p.get(); */
    /* {
        shared_ptr<int> (p.get());
    }
    int foo = *p;
    cout << "foo is " << foo << endl; */

    if(p)
        cout << "p is not null" << endl;
    else
        cout << "p is null" << endl;

    auto p1 = pInt(new int(42));
    if(p1)
        cout << "p is not null" << endl;
    else
        cout << "p is null" << endl;
}

