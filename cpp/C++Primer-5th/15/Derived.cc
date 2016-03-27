/*
 *
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

class Bulk_quote : public Quote{
    Bulk_quote() = default;
    Bulk_quote(const std::string&, double, std::size_t, double);
    double net_price(std::size_t) const override;
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};
