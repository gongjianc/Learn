/*
** @file    utils.cpp
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 10:36:35
*/
 
#include "utils.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

void get_files_in_dir(const char *dir, int depth, std::vector<std::string> &files)
{
    //传入指针为空判断
    if(NULL == dir){
        fprintf(stderr, "dir param error\n");
        return;
    }
    DIR *pDir = opendir(dir);
    if(NULL == pDir){
        //输出到给定的output stream
        fprintf(stderr, "cannot open directory:%s\n", dir);
        return;
    }
    chdir(dir);
    struct dirent *pEntry;
    struct stat statBuf;
    while(NULL != (pEntry = readdir(pDir))){
        //?
        stat(pEntry->d_name, &statBuf);
        if(S_ISDIR(statBuf.st_mode)){
            if(strcmp(".", pEntry->d_name) == 0 || strcmp("..", pEntry->d_name) == 0)
                continue;
            printf("%*s%s\n", depth, "", pEntry->d_name);
            get_files_in_dir(pEntry->d_name, depth + 4, files);
        }else{
            printf("%*s%s\n", depth, "", pEntry->d_name);
            std::string filename(dir);
            filename += '/';
            filename += pEntry->d_name;
            files.push_back(filename);
        }
    }
}

std::string upper_to_lower(std::string &word)
{
    std::size_t sz = word.size();
    if(sz > 0){
        std::string::iterator sit = word.begin();
        for(; sit != word.end(); ++sit){
            char ch = *sit;
            if(isalpha(ch)){
                if(isupper(ch))
                    *sit = tolower(*sit);
            }else{
                //不是字符，则反悔一个空字符串
                return std::string();
            }
        }
    }
    return word;
}


