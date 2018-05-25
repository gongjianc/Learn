#include <iostream>

using namespace std;

class PrintString{
public:
    PrintString(ostream &o = cout, char c = ' ')
    : os(o)
    , sep(c)
    {}
    void operator() (const string &s) const { os << s << sep; }
private:
    ostream &os;
    char sep;
};

int main()
{
    string s = {"hello,world\n"};
    PrintString printer;
    printer(s);
    PrintString error(cerr, '\n');
    error(s);
}
