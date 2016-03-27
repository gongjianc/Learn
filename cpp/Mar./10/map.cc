/*
** @file    map.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-04 19:23:30
*/
 
#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::endl;
using std::string;
using std::map;//map 中的数据也是唯一的
using std::pair;

int test1(void)
{
    pair<string, string> pairArr[4] =
    {
        pair<string, string>("A", "Asia"),
        pair<string, string>("B", "America"),
        pair<string, string>("C", "Euro"),
        pair<string, string>("D", "Afirca")
    };

    map<string, string> mapStr(pairArr, pairArr + 4);
    map<string, string>::const_iterator mit;
    for(mit = mapStr.cbegin(); mit != mapStr.cend(); ++mit)
        cout << mit->first << "--->" << mit->second << endl;
    cout << endl;

    cout << mapStr[string("C")] << endl;
    return 0;
}

int main(void)
{
    pair<int, string> t(9, "Asia");
    //cout << t.first << "-->" << t.second << endl; //介绍pair

    pair<int, string> pairArr[4] = 
    {
        pair<int, string>(9, "Asia"),
        pair<int, string>(4, "America"),
        pair<int, string>(1, "Euro"),
        pair<int, string>(4, "Africa") //4 会冲突 过滤掉
    };

    map<int, string> mapInt2Str(pairArr, pairArr + 4);
    cout << "map's size = " << mapInt2Str.size() << endl;

    map<int, string>::const_iterator mit;
    for(mit = mapInt2Str.cbegin(); mit != mapInt2Str.cend(); ++mit)
        cout << mit->first << "-->" << mit->second << endl;

    cout << endl;

    return 0;
}
