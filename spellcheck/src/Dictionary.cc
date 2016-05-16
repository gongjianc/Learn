/*
** @file    Dictionary.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-27 02:45:19
*/
 
#include "Dictionary.h"
#include <fstream>
#include <iostream>

namespace my{

void Dictionary::readFile(std::string filename)
{
    std::ifstream ifs(filename);
    std::string word;
    while(ifs >> word){
        if(_mapDict.find(word) != _mapDict.end())
            _mapDict[word]++;
        else{
            _mapDict[word] = 1;
        }
    }
}

void Dictionary::writeFile(std::string filename)
{
    std::ofstream ofs(filename);
    std::map<std::string, int>::iterator it;
    for(it = _mapDict.begin(); it != _mapDict.end(); ++it){
        ofs << it->first << " " 
            << it->second << std::endl;
    }
}




}// namespace my
