#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include "Configuration.hpp"
#include <iostream>
#include <vector>
#include <string>

namespace wd{
class SplitTool{
public:
    SplitTool() {}
    virtual ~SplitTool(){}
    virtual std::vector<std::string> cut(const std::string &sentence) = 0;
};

class SplitToolNLPIR : public SplitTool{
public:
    SplitToolNLPIR(wd::Configuration &conf);
    ~SplitToolNLPIR();
    std::vector<std::string> cut(const std::string &sentence);
private:
    wd::Configuration & _conf;
};
};//end of namespace
#endif
