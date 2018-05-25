/*
** @file    RAII.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-10 23:15:46
*/
 
#include <iostream>
 
using std::cout;
using std::endl;

//Resource Allocate is Initialization
template <typename T>
class RAII
{
public:
    RAII(T *p)
    : _pVal(p)
    {
        cout << "RAII(T *p)" << endl;
    }
    T *get() const
    {
        return _pVal;
    }
    T &operator*()
    {
        return *_pVal;
    }
    void reset(T *p)
    {
        delete _pVal;
        _pVal = p;
    }
    ~RAII()
    {
        cout << "~RAII()" << endl;
        delete _pVal;
    }
private:
    RAII(const T &);//复制构造函数
    RAII &operator=(const T &);
private:
    T *_pVal;
};

class SomeResourse
{
};

class Example
{
public:
    Example()
    : _e1(new SomeResourse())
    , _e2(new SomeResourse())
    {}
private:
    RAII<SomeResourse> _e1;
    RAII<SomeResourse> _e2;
};

int main(void)
{
    Example example;
    return 0;
}
