#ifndef __MY_FACTORY_H__
#define __MY_FACTORY_H__

class Product{
public:
    Product(){}
    virtual ~Product(){}
};

class ConcreteProduct : public Product{
public:
    ConcreteProduct(){}
    ~ConcreteProduct(){}
};

class Creator{
public:
    Creator(){}
    virtual ~Creator(){}
    void anOperation();
protected:
    virtual Product *factoryMethod() = 0;
};

class ConcreteCreator : public Creator{
public:
    ConcreteCreator(){}
    virtual ~ConcreteCreator(){}
protected:
    virtual Product *factoryMethod();
};

#endif
