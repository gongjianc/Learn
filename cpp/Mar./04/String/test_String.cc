/*
** @file    test_String.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-04 06:52:40
*/
 
#include "String.hh"
 
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

void display(int i, const String &str)
{
    cout << i << "--------------" << endl
         << "string" << i
         << " = " << str 
         << endl << endl;
}


int main(void)
{
    String string1;
    display(1, string1);
    
    String string2("Hello");
    display(2, string2);

    String string3(string2);
    display(3, string3);

    string1 = "Wolrd";
    display(1, string1);

    string2 += string1;
    display(2, string2);

    string3 += "world";
    display(3, string3);

    cout << "string3[0] = " << string3[0] << endl << endl;
    
    cout << "string3.size() = " << string3.size() << endl << endl;

    const String string4 = string3;
    display(4, string4);
    cout << "string4.c_str() = " << string4.c_str() << endl << endl;

    cout << "string1 == string4 ? " << (string1 == string4) << endl << endl;
    cout << "string4 == string4 ? " << (string4 == string4) << endl << endl;
    
    return 0;
}


