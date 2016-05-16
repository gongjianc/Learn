/*
** @file    test.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-27 01:21:32
*/
 
#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"
 
using std::cout;
using std::endl;

int main(void)
{
    my::Dictionary dict;
    std::ifstream ifs("my.conf");
    if(!ifs.good()){
        cout << "ifstream open error" << endl;
        return -1;
    }
    std::string word;
    while(ifs >> word){
        if(word == "source"){
            ifs >> word;
            std::string src = word.c_str();
            dict.readFile(src);
            cout << "source = " << src << endl;
        }else if(word == "destination"){
            ifs >> word;
            std::string dest = word.c_str();
            dict.writeFile(dest);
            cout << "destination = " << dest << endl;
        }
    }
    ifs.close();


    return 0;
}
