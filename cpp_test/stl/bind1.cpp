#include <functional>
#include <iostream>
#include <list>
#include "print.hpp"
#include <algorithm>

using namespace std;
using namespace std::placeholders;

int main()
{
    list<int> listTmp = {1, 2, 3, 4, 5};
    auto plus10 = bind(plus<int>(), _1, 10);
    cout << "7 + 10 = " << plus10(7) << endl;
    PRINT_ELEMENTS(listTmp, "Start --- ");
    /* for_each(listTmp.begin(), listTmp.end(), plus10); */
    //这里运行结束后，为何没有+1
    /* for_each(listTmp.begin(), listTmp.end(), bind(plus<int>(), _1, 10)); */
    /* for_each(listTmp.begin(), listTmp.end(), bind(plus10, _1)); */
    PRINT_ELEMENTS(listTmp, "End ---- ");
}
