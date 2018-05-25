/*
** @file    associative_container.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-09 17:27:00
*/
 
#include <iostream>
#include <map>
#include <string>
 
using std::cout;
using std::endl;
using std::map;
using std::multimap;
using std::string;
using std::pair;

void display(map<int, string> &mapIntStr)
{
    map<int, string>::iterator mit;
    for(mit = mapIntStr.begin(); mit != mapIntStr.end(); ++mit){
        cout << mit->first << "-->"
             << mit->second << endl;
    }
    cout << endl;
}
int main(void)
{
    typedef map<int, string>::value_type mValueType;
    mValueType arr[3] = 
    {
        mValueType(1, "A"),
        mValueType(2, "B"),
        mValueType(3, "C")
    };

    map<int, string> mapInt2Str(arr, arr + 3);
    display(mapInt2Str);

    map<int, string>::size_type num = mapInt2Str.erase(1);
    cout << "删除所有关键字为1的元素后：" << endl;
    display(mapInt2Str);
    cout << num << endl;



    return 0;
    
}
