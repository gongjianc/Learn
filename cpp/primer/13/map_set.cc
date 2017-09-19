/*
** @file    map.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-16 23:19:52
*/
 
#include <iostream>
#include <map>
#include <set>
#include <string>
 
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::cin;
using std::set;

int main(void)
{
    map<string, std::size_t> word_count;
    set<string> exclude =
    {
        "The", "But", "And", "Or", "An", "A",
        "the", "but", "and", "or", "an", "a"
    };
    string word;
    cout << "word_count.size = " << word_count.size() << endl;
    while(cin >> word)
        if(exclude.find(word) == exclude.end())
            ++word_count[word];

    cout << "word_count.size = " << word_count.size() << endl;
    for(const auto &w : word_count)
        cout << w.first << " occurs " << w.second
             << ((w.second > 1) ? "times" : "time" ) << endl;

    return 0;
}


