#include <list>
#include <algorithm>
#include <iostream>
#include "print.hpp"

using namespace std;

class AddValue{
private:
    int _theValue;
public:
    AddValue(int v) : _theValue(v){}
    void operator()(int &elem) const {
        elem += _theValue;
    }
};

int main()
{
    list<int> coll;
    for (int i = 1; i <=9; i++)
        coll.push_back(i);
    PRINT_ELEMENTS(coll, "initialized:     ");
    /*
     * for_each
     */
    for_each(coll.begin(), coll.end(), AddValue(10));
    PRINT_ELEMENTS(coll, "after adding 10:     ");
    for_each(coll.begin(), coll.end(), AddValue(*coll.begin()));
    PRINT_ELEMENTS(coll, "after adding first ele:     ");
}
