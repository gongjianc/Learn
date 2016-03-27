/*
** @file    test_observer.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-20 22:20:18
*/
 
#include "observer.h"
#include <iostream>
 
using std::cout;
using std::endl;

int main(void)
{
    Observer *p1 = new ConcreteObserver;
    Observer *p2 = new ConcreteObserver;

    Subject *pSubject = new ConcreteSubject;
    pSubject->attach(p1);
    pSubject->attach(p2);
    pSubject->setState(4);
    pSubject->notify();

    cout << endl;

    pSubject->detach(p2);
    pSubject->setState(10);
    pSubject->notify();

    delete p1;
    delete p2;
    delete pSubject;

    return 0;
}
