/*
** @file    MyTask.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 16:37:05
*/
 
#include <iostream>
#include "MyTask.h"
#include "MyDict.h"
 
MyTask::MyTask(const std::string &queryWord, int peerfd)
    :_queryWord(queryWord)
    ,_peerfd(peerfd)
{}

void MyTask::execute()
{
    std::cout << "Task::execute()-->query" << _queryWord << std::endl;
    MyDict *pDict = MyDict::createInstance();
    auto myDict = pDict->get_dict();
    //只为测试
    std::cout << myDict[0].first << "-->" << myDict[0].second << std::endl;
}
