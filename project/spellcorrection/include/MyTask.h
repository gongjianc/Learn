#ifndef __MYTASK_H__
#define __MYTASK_H__

#include <string>
#include <vector>
#include <queue>
#include <set>

struct MyResult{
    std::string _word;
    int _iFreq;
    int _iDist;
};

//策略
struct MyCompare{
    bool operator()(const MyResult &lhs, const MyResult &rhs){
        if(lhs._iDist > rhs._iDist){
            return true;
        }else if(lhs._iDist == rhs._iDist && lhs._iFreq < rhs._iFreq){
            return true;
        }else if(lhs._iDist == rhs._iDist && lhs._iFreq == rhs._iFreq && lhs._word > rhs._word){
            return true;
        }else{
            return false;
        }
    }
};

class Cache;
class MyTask{
public:
    MyTask(const std::string &queryWord, int peerfd);
    void execute(Cache &cache);
private:
    //查询索引表
    void queryIndexTable();
    void statistic(std::set<int> &iset);
    int distance(const std::string &rhs);
    //给客户端返回
    void response(Cache &cache);
private:
    std::string _queryWord;
    int _peerfd;
    std::priority_queue<MyResult, std::vector<MyResult>, MyCompare> _resultQue;
};

#endif
