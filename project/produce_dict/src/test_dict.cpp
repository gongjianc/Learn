/*
** @file    test_dict.cpp
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 10:57:42
*/
 
#include "Configuration.hpp"
#include "DictProducer.hpp"
#include "SplitTool.h"
#include <iostream>
#include <string>
#include <vector>
 
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(void)
{
    wd::Configuration config("../conf/my.conf");
    auto map = config.getConfigMap();
    for(auto &elem : map)
        cout << elem.first << "-->"
             << elem.second << endl;

    wd::DictProducer producer(map["yuliao"]);
    producer.show_files();
    producer.build_dict();
    producer.store_dict(map["dict"].c_str());

    return 0;
}

