/*
** @file    word_transformation.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-17 04:43:22
*/
 
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>
#include <sstream>
 
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::istringstream;
using std::map;
using std::runtime_error;

map<string, string> buildMap(ifstream &map_file)
{
    map<string, string> trans_map;
    string key;
    string value;
    while(map_file >> key && getline(map_file, value))
        if(value.size() > 1)
            trans_map[key] = value.substr(1);//跳过第一个空格
        else
            throw runtime_error("no rule for " + key);
    return trans_map;
}

const string &
transform(const string &s, const map<string, string> &m)
{
    auto map_it = m.find(s);
    if(map_it == m.cend())
        return s;
    else 
        //dereference the iterator, wo get a pair that hold 
        //the key and value for the element.
        return map_it->second;
}

void word_transformation(ifstream &map_file, ifstream &input)
{
    auto trans_map = buildMap(map_file);
    string text;
    while(getline(input, text)){
        //
        istringstream stream(text);
        string word;
        bool firstword = true;
        while(stream >> word){
            if(firstword)
                firstword = false;
            else
                cout << " ";
            cout << transform(word, trans_map);
        }
        cout << endl;
    }
}

int main(void)
{
    ifstream map_file("map_file.dat", std::ios::in);
    ifstream input("input.txt", std::ios::in);
    word_transformation(map_file, input);
    return 0;
}
