#ifndef __MY_DICTIONARY_H__
#define __MY_DICTIONARY_H__

#include <string>
#include <map>

namespace my{

class Dictionary{
public:
    Dictionary();
    //Dictionary(std::string filename);
    void readFile(std::string filename);
    void writeFile(std::string filename);

private:
    std::map<std::string, int> _mapDict;
};

}// namespace my

#endif
