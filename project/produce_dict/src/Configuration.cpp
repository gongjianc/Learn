/*
** @file    Configuration.cpp
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-16 23:07:16
*/

#include "Configuration.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
 
namespace wd{

Configuration::Configuration(const std::string &filepath)
    : _filepath(filepath)
{
    readConfiguration();
}

void Configuration::readConfiguration()
{
    //转换为 C 风格字符串
    std::ifstream ifs(_filepath.c_str());
    if(ifs.good()){
        std::string line;
        while(getline(ifs, line)){
            std::istringstream iss(line);
            std::string key;
            std::string value;
            iss >> key >> value;
            _configMap[key] = value;
        }
        std::cout << "read Configuration is over!" << std::endl;
    }else{
        std::cout << "ifstream open error" << std::endl;
    }
}

std::map<std::string, std::string> &Configuration::getConfigMap()
{
    return _configMap;
}

};
