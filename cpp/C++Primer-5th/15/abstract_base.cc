/*
** @file    Derived.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-15 07:31:41
*/
 
#include <iostream>
#include <string>
 
using std::cout;
using std::endl;

class Quote{
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price)
    : _bookNo(book)
    , _price(sales_price)
    {}
    std::string isbn() const
    { return _bookNo; }
    virtual double net_price(std::size_t n) const
    { return n * _price; }
    virtual ~Quote() = default;
private:
    std::string _bookNo;
protected:
    double _price = 0.0;
};

class Disc_quote : public Quote {
public:
    Disc_quote() = default;
    Disc_quote(const std::string &book, double price, std::size_t qty, double disc)
        : Quote(book, price)
    , _quantity(qty)
    , _discount(disc)
    {}
    double net_price(std::size_t) const = 0; //"=0" pure virtual
protected:
    std::size_t _quantity = 0;
    double _discount = 0.0;
};

int main(void)
{
    //不能生成一个abstract object
    //Disc_quote dq;
    return 0;
}
