#ifndef __MY_CACHE_H__
#define __MY_CACHE_H__

#include <iostream>
#include <string>
#include <unordered_map>

class Cache{
public:
    Cache(int num = 100);
    Cache(const Cache &cache);

    void addElement(const std::string &key, const std::string &value);
    std::string query(const std::string &word);
    void readFromFile(const std::string &filename);
    void writeToFile(const std::string &filename);
    void update(const Cache &rhs);
private:
    std::unordered_map<std::string, std::string> _hashMap;
};

#endif
