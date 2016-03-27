/*
** @file    observer.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-20 20:46:22
*/
 
#include "observer.h"
#include <iostream>
#include <algorithm>

void Subject::attach(Observer *pObserver)
{
    std::cout << "attach on observer" << std::endl;
    _listObserve.push_back(pObserver);
}

void Subject::detach(Observer *pObserver)
{
    std::list<Observer *>::iterator iter;
    iter = std::find(_listObserve.begin(), _listObserve.end(), pObserver);

    if(_listObserve.end() != iter)
        _listObserve.erase(iter);

    std::cout << "detach an Observer" << std::endl;
}

void Subject::notify()
{
    std::cout << "notify Observer's State" << std::endl;
    std::list<Observer *>::iterator iter;
    for(iter = _listObserve.begin(); iter != _listObserve.end(); ++iter)
        (*iter)->update(this);
}

void Subject::setState(STATE state)
{
    std::cout << "setState by Subject" << std::endl;
    _subjectState = state;
}

STATE Subject::getState()
{
    std::cout << "getState by Subject" << std::endl;
    return _subjectState;
}

Subject::~Subject()
{
    _listObserve.clear();
}

/*------------------------------
|* ConcreteSubject
|* ---------------------------*/

void ConcreteSubject::setState(STATE state)
{
    std::cout << "setState by ConcreteSubject" << std::endl;
    _subjectState = state;
}

STATE ConcreteSubject::getState()
{
    std::cout << "getState by ConcreteSubject" << std::endl;
    return _subjectState;
}

/*=============================*/

//void ConcreteObserver::update(std::string &msg);//推模式
void ConcreteObserver::update(Subject *pSubject)//拉模式
{
    if(NULL == pSubject)
        return;

    _observerState = pSubject->getState();
    std::cout << "the observerState is " << _observerState << std::endl;
}



