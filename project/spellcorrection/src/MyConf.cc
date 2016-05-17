/*
** @file    MyConf.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 15:56:28
*/
 
#include "MyConf.h"
#include <iostream>
#include <fstream>
#include <utility>

MyConf::MyConf(const std::string &filepath)
    : _filepath(filepath)
{
    init();
}

bool MyConf::init()
{
    std::ifstream ifs(_filepath.c_str());
    if(!ifs){
        std::cout << "file read error" << std::endl;
        return false;
    }
    std::string line;
    while(getline(ifs, line)){
        std::istringstream iss(line);
        std::string key, value;
        iss >> key >> value;
        _map.insert(make_pair(key, value));
    }
    ifs.close();
    return true;
}

void MyConf::show()
{
    std::map<std::string, std::string>::const_iterator mit = _map.begin();
    for(; mit != _map.end(); ++mit)
        std::cout << mit->first << "--->"
                  << mit->second << std::endl;
}
