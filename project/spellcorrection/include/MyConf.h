#ifndef __MYCONF_H__
#define __MYCONF_H__

#include <string>
#include <sstream>
#include <map>

class MyConf{
public:
    MyConf(const std::string &);
    bool init();
    std::map<std::string, std::string> &getMap()
    {
        return _map;
    }
    void show();
private:
    std::string _filepath;
    std::map<std::string, std::string> _map;
};
#endif
