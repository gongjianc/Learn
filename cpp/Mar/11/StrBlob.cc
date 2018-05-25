/*
** @file    StrBlob.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-11 05:03:35
*/
 
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
 
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class StrBlob{
public:
    typedef vector<string>::size_type size_type;
    StrBlob()
    : _data(make_shared<vector<string> >())
    {
        cout << "StrBlob()" << endl;
    }
    StrBlob(std::initializer_list<string> il)
    : _data(make_shared<vector<string> >(il))//make_shared 
    {
        cout << "StrBlob(initializer_list<string>)" << endl;
    }
    size_type size() const
    { return _data->size(); }                                                                                                                                                                                                                                                                                                                           
    bool empty() const                                                                                                                                                                                                                                      
    { return _data->empty(); }
    void push_back(const string &t)
    { _data->push_back(t); }
    void pop_back();                                                                                                                                                                                               
    string &front();
    string &back();
private:
    shared_ptr<vector<string> > _data;
    void check(size_type i, const string &msg) const;
};


void StrBlob::check(size_type i, const string &msg) const
{
    if(i >= _data->size())
        throw std::out_of_range(msg);
}

string &StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return _data->front();
}

string &StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return _data->back();
}

void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    _data->pop_back();
}


int main(void)
{
    StrBlob b1;
    cout << "b1.size() = " << b1.size() << endl;
    {
        StrBlob b2 = {"a", "an", "the"};
        b1 = b2;
        b2.push_back("about");
        cout << "b2.size() = " << b2.size() << endl;
    }
    cout << "b1.size() = " << b1.size() << endl;
    //cout << "b2.size() = " << b2.size() << endl;
    while(b1.size()){
        cout << b1.back() << endl;
        b1.pop_back();
    }
    cout << "b1.size() = " << b1.size() << endl;


    return 0;
}
