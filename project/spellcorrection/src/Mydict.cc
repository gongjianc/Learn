/*
** @file    Mydict.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 14:43:19
*/
 
#include "MyDict.h"
#include <iostream>
#include <fstream>
#include <sstream>

//静态变量
MyDict *MyDict::pInstance = NULL;

MyDict *MyDict::createInstance()
{
    if(NULL == pInstance)
        pInstance = new MyDict;
    return pInstance;
}

std::vector<std::pair<std::string, int> > &MyDict::get_dict()
{
    return _dict;
}
 
std::map<std::string, std::set<int> > &MyDict::get_index_table()
{
    return _index_table;
}

void MyDict::read_from(const char *dictpath)
{
    std::ifstream in(dictpath);
    if(!in){
        std::cout << __DATE__ << " " << __TIME__
                  << __FILE__ << " " << __LINE__
                  << ": dict read error" << std::endl;
        exit(-1);
    }
    
    std::string line;
    while(getline(in, line)){
        std::stringstream ss(line);
        std::string key;
        int value;
        ss >> key >> value;
        _dict.push_back(make_pair(key, value));
    }
    in.close();
}

void MyDict::init(const char *dictpath, const char *cnDictpath)
{
    read_from(dictpath);
    //中文
    read_from(cnDictpath);
    //创建索引表
    for(std::size_t idx = 0; idx != _dict.size(); ++idx)
        record_to_index(idx);
}

void MyDict::record_to_index(int idx)
{
    std::string key;
    //_dict: vector<pair<string, int> >
    std::string word = _dict[idx].first;
    for(std::size_t iidx = 0; iidx != word.size(); ++iidx){
        char ch = word[iidx];
        //针对中文字符的处理
        if(ch & (1 << 7)){
            if((ch & 0xF0) == 0xC0 || (ch & 0xF0) == 0xD0){
                key = word.substr(iidx, 2);
                ++iidx;
            }
        }else{
            key = word.substr(iidx, 1);
        }
        //_index_table: map<string, set<int> >
        _index_table[key].insert(idx);
    }
}














