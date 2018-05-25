/*
** @file    Buffer.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-21 05:19:39
*/
 
#include "Buffer.h"

namespace wd{

Buffer::Buffer(std::size_t size)
: _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _size(size)
{
}

bool Buffer::full()
{
    return _size == _que.size();
}

bool Buffer::empty()
{
    return 0 == _que.size();
}

void Buffer::push(int val)
{
    _mutex.lock();
    //为什么不用if
    while(full())
        _notFull.wait();

    _que.push(val);
    _notEmpty.notify();
    _mutex.unlock();
}

int Buffer::pop()
{
    _mutex.lock();
    while(empty())
        _notEmpty.wait();
    int temp = _que.front();
    _que.pop();
    _notFull.notify();
    _mutex.unlock(); //
    return temp;
}

}//end of namespace
