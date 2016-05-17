#ifndef __MYDICT_H__
#define __MYDICT_H__

#include <string>
#include <vector>
#include <set>
#include <utility>
#include <map>

class MyDict{
public:
    static MyDict *createInstance();
    void init(const char *dictpath, const char *cnDictpath);
    void show_dict();
    void show_index_table();

    std::vector<std::pair<std::string, int> > &get_dict();
    std::map<std::string, std::set<int> > &get_index_table();
private:
    //单例模式
    MyDict() {}
    void read_from(const char *dictpath);
    void record_to_index(int idx);
private:
    static MyDict *pInstance;
    std::vector<std::pair<std::string, int> > _dict;
    std::map<std::string, std::set<int> > _index_table;
};

#endif
