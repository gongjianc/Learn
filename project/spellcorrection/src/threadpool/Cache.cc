/*
** @file    Cache.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-19 17:21:20
*/

#include "Cache.h"
#include <iostream>
#include <fstream>

Cache::Cache(int num)
: _hashMap(num)
{}

Cache::Cache(const Cache &rhs)
: _hashMap(rhs._hashMap)
{}

void Cache::addElement(const std::string &key, const std::string &value)
{
    _hashMap[key] = value;
}

std::string Cache::query(const std::string &word)
{
    auto iter = _hashMap.find(word);
    if(iter != _hashMap.end())
        return iter->second;
    //找不到返回空串
    else
        return std::string();
}

void Cache::readFromFile(const std::string &filename)
{
    std::ifstream ifs(filename.c_str());
    if(!ifs.good()){
        std::cout << "open cache file error!" << std::endl;
        return;
    }

    std::string key, value;
    while(ifs >> key >> value){
        _hashMap.insert(std::make_pair(key, value));
        std::cout << key << " " << value << std::endl;
    }
}

void Cache::writeToFile(const std::string &filename)
{
    std::ofstream ofs(filename.c_str());
    if(!ofs.good()){
        std::cout << "write cache file error!" << std::endl;
        return;
    }

    auto iter = _hashMap.begin();
    for(; iter != _hashMap.end(); ++iter){
        ofs << iter->first << "\t" << iter->second << std::endl;
    }
}

void Cache::update(const Cache &rhs)
{
    auto iter = rhs._hashMap.begin();
    for(; iter != rhs._hashMap.end(); ++iter)
        //hashMap的 insert 重复？
        _hashMap.insert(*iter);
}
