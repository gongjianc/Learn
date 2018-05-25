/*
** @file    shared_ptr.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 01:26:16
*/
 
#include <iostream>
#include <memory>
 
using std::cout;
using std::endl;
using std::shared_ptr;

class Parent;
class Child;

class Parent
{
public:
    Parent()
    {
        cout << "Parent()" << endl;
    }
    ~Parent()
    {
        cout << "~Parent()" << endl;
    }
//private:
    shared_ptr<Child> _child;
};

class Child
{
public:
    Child()
    {
        cout << "Child()" << endl;
    }
    ~Child()
    {
        cout << "~Child()" << endl;
    }
//private:
    shared_ptr<Parent> _parent;
};

int main(void)
{
    shared_ptr<Parent> parent(new Parent);
    shared_ptr<Child> child(new Child);
    cout << "parent's use_count = " << parent.use_count() << endl;
    cout << "child's use_count = " << child.use_count() << endl;
    parent->_child = child;
    child->_parent = parent; //循环引用
    cout << "parent's use_count = " << parent.use_count() << endl;
    cout << "child's use_count = " << child.use_count() << endl;
    return 0;
}

