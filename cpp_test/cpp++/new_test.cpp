#include <iostream>
#include <string>

using namespace std;

int main()
{
    int *pi = new int;
    string *ps  = new string();
    auto p2 = new auto("hello");
    cout << "pi is " << *pi << endl;
    cout << "ps is " << *ps << endl;
    cout << "p2 is " << *p2 << endl << "p2 type is " << endl;
}
