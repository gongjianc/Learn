#ifndef __MYTASK_H__
#define __MYTASK_H__

#include <string>

class MyTask{
public:
    MyTask(const std::string &queryWord, int peerfd);
    void execute();
private:
    std::string _queryWord;
    int _peerfd;
};

#endif
