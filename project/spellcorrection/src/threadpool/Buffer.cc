/*
** @file    Buffer.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-18 16:39:37
*/

#include "Buffer.h"

namespace wd{

Buffer::Buffer(int size)
: _size(size)
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
{}

Buffer::~Buffer()
{}

bool Buffer::full()
{
    return _size == (int)_que.size();
}

bool Buffer::empty()
{
    return (0 == _que.size());
}

void Buffer::push(Task ptask)
{
    MutexLockGuard mlg(_mutex);
    while(full())
        _notFull.wait();
    _que.push(ptask);
    _notEmpty.notify();
}

Buffer::Task Buffer::pop()
{
    MutexLockGuard mlg(_mutex);
    while(empty())
        _notEmpty.wait();
    Task ptask = _que.front();
    _que.pop();
    _notFull.notify();
    return ptask;
}

}
