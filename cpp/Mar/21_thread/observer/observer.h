#ifndef __MY_OBSERVER_H__
#define __MY_OBSERVER_H__

#include <list>
typedef int STATE;

class Observer;

class Subject{
public:
    Subject() : _subjectState(-1){}
    virtual ~Subject();

    void notify();
    //新增对象
    void attach(Observer *pObserver);
    //删除对象
    void detach(Observer *pObserver);
    virtual void setState(STATE state);
    virtual STATE getState();
protected:
    STATE _subjectState;
    std::list<Observer *> _listObserve;
};

class ConcreteSubject : public Subject{
public:
    ConcreteSubject(){}
    virtual ~ConcreteSubject(){}
    virtual void setState(STATE state);
    virtual STATE getState();
};
/*============================================================*/
class Observer{
public:
    Observer() : _observerState(-1){}
    virtual ~Observer(){}
    
    virtual void update(Subject *pSubject) = 0;
protected:
    STATE _observerState;
};

class ConcreteObserver : public Observer{
public:
    ConcreteObserver() : Observer(){}
    virtual ~ConcreteObserver(){}
    virtual void update(Subject *pSubject);
};

#endif
