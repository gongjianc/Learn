/*
** @file    DictProducer.hpp
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-16 23:21:36
*/
 
#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "SplitTool.h"
#include <string>
#include <vector>
#include <map>

namespace wd{
class DictProducer{
public:
    DictProducer(const char *dir);
    DictProducer(const std::string &dir);
    //中文词典
    DictProducer(const std::string &dir, SplitTool *splitTool);

    void build_dict();
    void build_cn_dict();
    void show_files() const;
    void show_dict() const;
    void store_dict(const char*filepath);
private:
    void get_files();
    void push_dict(const std::string &word);
private:
    std::string _dir;
    std::vector<std::string> _files;
    std::map<std::string, int> _dict;
    SplitTool *_splitTool;
};
};

#endif

