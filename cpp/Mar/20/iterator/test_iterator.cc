/*
** @file    test_iterator.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-20 08:34:39
*/
 
#include <iostream>
#include "iterator.h"
 
using std::cout;
using std::endl;

int main(void)
{
    Aggregate *pAggregate = new ConcreteAggregate(4);
    Iterator *pIt = pAggregate->createIterator();

    for(; false == pIt->isDone(); pIt->next())
        cout << pIt->currentItem() << endl;

    delete pAggregate;
    delete pIt;

    return 0;
}
