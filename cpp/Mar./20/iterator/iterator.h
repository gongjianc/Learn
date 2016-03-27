#ifndef __MY_ITERATOR_H__
#define __MY_ITERATOR_H__

class Iterator;
//为什么？
typedef int DATA;


class Aggregate{
public:
    virtual Iterator *createIterator() = 0;
    virtual ~Aggregate() {}

    virtual int size() = 0;
    virtual DATA getItem(int) = 0;
};

class Iterator{
public:
    virtual ~Iterator() {}
    virtual void first() = 0;
    virtual void next() = 0;
    virtual bool isDone() = 0;
    virtual DATA currentItem() = 0;
};

class ConcreteAggregate : public Aggregate{
public:
    ConcreteAggregate(int);
    virtual Iterator *createIterator();
    virtual ~ConcreteAggregate();

    virtual int size();
    virtual DATA getItem(int);
private:
    int _size;
    DATA *_pData;
};

class ConcreteIterator : public Iterator{
public:
    ConcreteIterator(Aggregate *);
    virtual ~ConcreteIterator(){}

    virtual void first();
    virtual void next();
    virtual bool isDone();
    virtual DATA currentItem();
private:
    int _idx;
    Aggregate *_pAggregate;
};


#endif
