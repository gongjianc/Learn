/*
** @file    array_wrapper.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-10 19:00:00
*/
 
#include <iostream>
#include <string>
 
using std::cout;
using std::endl;
using std::string;

class MetaData
{
public:
    MetaData(int size, const string &name)
    : _name(name)
    , _size(size)
    {
        cout << "MetaData(int, const string &)" << endl;
    }
    MetaData(const MetaData &rhs)
    : _name(rhs._name)
    , _size(rhs._size)
    {
        cout << "MetaData(const MetaData &)" << endl;
    }

    MetaData(MetaData &&rhs)
    : _name(rhs._name)
    , _size(rhs._size)
    {
        cout << "MetaData(MetaData &&)" << endl;
    }
    string getName() const
    {
        return _name;
    }
    int getSize() const
    {
        return _size;
    }
private:
    string _name;
    int _size;
};

class ArrayWrapper
{
public:
    ArrayWrapper()
    : _pVals(new int[64])
    , _metaData(64, "ArrayWrapper")
    {
        cout << "ArrayWrapper()" << endl;
    }
    ArrayWrapper(int n)
    : _pVals(new int[n])
    , _metaData(n, "ArrayWrapper")
    {
        cout << "ArrayWrapper(int)" << endl;
    }
    ArrayWrapper(const ArrayWrapper &rhs)
    : _pVals(new int[rhs._metaData.getSize()])
    , _metaData(rhs._metaData)
    {
        cout << "ArrayWrapper(const ArrayWrapper &)" << endl;
        for(int idx = 0; idx != _metaData.getSize(); ++idx)
        {
            _pVals[idx] = rhs._pVals[idx];
        }
    }

    ArrayWrapper(ArrayWrapper &&rhs)
    : _pVals(rhs._pVals)
    , _metaData(rhs._metaData)
    {
        cout << "ArrayWrapper(ArrayWrapper &&)" << endl;
        rhs._pVals = NULL;
    }

    ~ArrayWrapper()
    {
        delete[] _pVals;
        cout << "~ArrayWrapper()" << endl;
    }
private:
    int *_pVals;
    MetaData _metaData;
};

int main(void)
{
    ArrayWrapper aw1;
    ArrayWrapper aw2(std::move(aw1));//移动构造函数 aw1 已经作废
    //move是将左值转换为右值 
    return 0;
}
