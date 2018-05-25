#include <iostream>

int & fun()
{
    int *p = new int(5);
    std::cout << "p's addr is " << p << std::endl;

}
