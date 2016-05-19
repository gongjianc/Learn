/*
** @file    MyTask.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 16:37:05
*/
 
#include <iostream>
#include "MyTask.h"
#include "MyDict.h"
#include "Cache.h"
#include <unistd.h>
 
int tripleMin(const int &a, const int &b, const int &c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

MyTask::MyTask(const std::string &queryWord, int peerfd)
    :_queryWord(queryWord)
    ,_peerfd(peerfd)
{}

void MyTask::execute(Cache &cache)
{
#if 0
    std::cout << "Task::execute()-->query" << _queryWord << std::endl;
    MyDict *pDict = MyDict::createInstance();
    auto myDict = pDict->get_dict();
    //只为测试
    std::cout << myDict[0].first << "-->" << myDict[0].second << std::endl;
#endif
    std::string result = cache.query(_queryWord);
    if(result != std::string()){
        ::write(_peerfd, result.c_str(), result.size());
        std::cout << "Cache response client" << std::endl;
        return;
    }
    queryIndexTable();
    response(cache);
}

void MyTask::queryIndexTable()
{
    auto indexTable = MyDict::createInstance()->get_index_table();
    std::string ch;
    for(std::string::size_type idx = 0; idx != _queryWord.size(); ++idx){
        ch = _queryWord.substr(idx, 1);
        //count?是什么？
        if(indexTable.count(ch)){
            std::cout << "indexTable has character : " << ch << std::endl;
            statistic(indexTable[ch]);
        }
    }
}

void MyTask::statistic(std::set<int> &iset)
{
    auto dict = MyDict::createInstance()->get_dict();
    auto iter = iset.begin();
    for(; iter != iset.end(); ++iter){
        std::string rhsWord = dict[*iter].first;
        int iDist = distance(rhsWord);
        //只有编辑距离小于3的才计入优先级队列
        //具体排序优先级队列里边会有体现
        //MyCompare 算法
        if(iDist < 3){
            //候选词
            MyResult res;
            res._word = rhsWord;
            res._iFreq = dict[*iter].second;
            res._iDist = iDist;
            _resultQue.push(res);
        }
    }
}

void MyTask::response(Cache &cache)
{
    if(_resultQue.empty()){
        std::string result = "no answer!\n";
        int nwrite = ::write(_peerfd, result.c_str(), result.size());
        if(-1 == nwrite)
            std::cout << "reponse error" << std::endl;
    }else{
        MyResult result = _resultQue.top();
        int nwrite = ::write(_peerfd, result._word.c_str(), result._word.size());
        if(-1 == nwrite)
            std::cout << "find but reponse error" << std::endl;
        cache.addElement(_queryWord, result._word);//更新缓存
        std::cout << "response : add Cache" << std::endl;
    }
    std::cout << "response client" << std::endl;
}

int MyTask::distance(const std::string &rhs)
{
    int lhslen = _queryWord.size();
    int rhslen = rhs.size();
    //二维数组
    int editDist[lhslen +1][rhslen + 1];
    for(int idx = 0; idx <= lhslen; ++idx)
        editDist[idx][0] = idx;
    for(int idx = 0; idx <=rhslen; ++idx)
        editDist[0][idx] = idx;

    std::string subLhs, subRhs;
    for(int i = 1; i <= lhslen; ++i){
        subLhs = _queryWord.substr(i - 1, 1);
        for(int j = 1; j <= rhslen; ++j){
            subRhs = rhs.substr(j - 1, 1);
            if(subLhs == subRhs){
                editDist[i][j] = editDist[i - 1][j - 1];
            }else{
                editDist[i][j] = tripleMin(
                        editDist[i][j -1] + 1,
                        editDist[i - 1][j] + 1,
                        editDist[i - 1][j - 1] + 1);
            }
        }
    }
    return editDist[lhslen][rhslen];
}
                                







