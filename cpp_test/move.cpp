#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void TestSTLObject()
{
    string str = "hello";
    vector<string> vec;
    vec.push_back(str);
    cout << "After copy, str is \"" << str << "\"\n";
    vec.push_back(move(str));
    cout << "After move, str is \"" << str << "\"\n";
    cout << "The contents of the vector are \"" << vec[0]
         << "\", \"" << vec[1] << "\"\n";
}

int main()
{
    TestSTLObject();
    return 0;
}
