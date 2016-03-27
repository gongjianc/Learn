/*
** @file    iterator.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-20 01:31:03
*/
 
#include "iterator.h"
#include <iostream>

using std::cout;
using std::endl;

ConcreteAggregate::ConcreteAggregate(int size)
: _size(size)
, _pData(new DATA[_size])
{
    cout << "ConcreteAggregate(int)" << endl;
    for(int idx = 0; idx != _size; ++idx){
        _pData[idx] = idx;
    }
}

Iterator *ConcreteAggregate::createIterator()
{
    return new ConcreteIterator(this);
}

ConcreteAggregate::~ConcreteAggregate()
{
    delete []_pData;
    cout << "~ConcreteAggregate()" << endl;
}

int ConcreteAggregate::size()
{
    return _size;
}

DATA ConcreteAggregate::getItem(int idx)
{
    return _pData[idx];
}

ConcreteIterator::ConcreteIterator(Aggregate *pAggregate)
: _idx(0)
, _pAggregate(pAggregate)
{
    cout << "ConcreteIterator(Aggregate *)" << endl;
}

void ConcreteIterator::first()
{
    _idx = 0;
}

void ConcreteIterator::next()
{
    if(_idx < _pAggregate->size())
        ++_idx;
}

bool ConcreteIterator::isDone()
{
    return _idx == _pAggregate->size();
}

DATA ConcreteIterator::currentItem()
{
    return _pAggregate->getItem(_idx);
}
