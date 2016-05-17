/*
** @file    Configuration.hpp
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-16 22:51:57
*/

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__
 
#include <set>
#include <map>
#include <string>

namespace wd{

class Configuration{
public:
    Configuration(const std::string &filepath);
    std::map<std::string, std::string> &getConfigMap();
private:
    void readConfiguration();
private:
    std::string _filepath;
    std::map<std::string, std::string> _configMap;
};

};

#endif
