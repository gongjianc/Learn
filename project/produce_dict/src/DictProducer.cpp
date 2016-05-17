/*
** @file    DictProducer.cpp
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-16 23:43:18
*/
 
#include "DictProducer.hpp"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <fstream>

namespace wd{
DictProducer::DictProducer(const char *dir)
    : _dir(dir)
{
    get_files();
}

DictProducer::DictProducer(const std::string &dir)
    : _dir(dir)
{
    get_files();
}

DictProducer::DictProducer(const std::string &dir, SplitTool *splitTool)
    : _dir(dir)
    , _splitTool(splitTool)
{
    get_files();
}

//private
void DictProducer::get_files()
{
    std::cout << "_dir : " << _dir << std::endl;
    ::get_files_in_dir(_dir.c_str(), 0, _files);
}

void DictProducer::push_dict(const std::string &word)
{
    //_dict map
    auto iter = _dict.find(word);
    //如果找到，则将frequency+1
    if(iter != _dict.end())
        ++(iter->second);
    else
        //没找到，则插入
        //make_pair使用?
        _dict.insert(make_pair(word, 1));
}

void DictProducer::build_dict()
{
    for(std::size_t idx = 0; idx != _files.size(); ++idx){
        //从文件流中读入
        std::ifstream ifs(_files[idx].c_str());
        if(!ifs){
            std::cout << "stream error, file does not exist or other" << std::endl;
            continue;
        }
        //字符串流
        std::stringstream ss;
        std::string line;
        while(getline(ifs, line)){
            //从 line 中读取一个字符串
            ss << line;
            std::string word;
            while(ss >> word){
                //只存储小写字母
                //并对不是字符的操作进行过滤
                word = ::upper_to_lower(word);
                //上一步操作对含有非字母的单词进行赋值空字符的操作
                if(word == std::string())
                    continue;
                push_dict(word);
            }
            ss.clear();
        }
    }
}

void DictProducer::build_cn_dict()
{
    while(1);
}

void DictProducer::show_files() const
{
    std::vector<std::string>::const_iterator vit;
    for(vit = _files.begin(); vit != _files.end(); ++vit)
        std::cout << *vit << std::endl;
}

void DictProducer::show_dict() const
{
    auto mit = _dict.begin();
    for(; mit != _dict.end(); ++mit){
        std::cout << mit->first
                  << ' '
                  << mit->second
                  << '\n';
    }
}

void DictProducer::store_dict(const char *filepath)
{
    std::ofstream ofs(filepath);
    if(!ofs){
        std::cout << "ofstream error" << std::endl;
        return;
    }
    std::stringstream ss;
    auto mit = _dict.begin();
    for(; mit != _dict.end(); ++mit){
        int ifreq = mit->second;
        std::string sec;
        ss << ifreq;
        ss >> sec;
        std::string line;
        line += mit->first;
        line += " ";
        line += sec;
        line += "\r\n";
        std::cout << line;
        ofs << line;
        ss.clear();
    }
}

};//end of namespace
