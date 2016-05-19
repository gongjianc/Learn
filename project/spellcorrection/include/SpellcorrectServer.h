#ifndef __MY_SPELLCORRECTSERVER_H__
#define __MY_SPELLCORRECTSERVER_H__

#include "MyConf.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include "TimerThread.h"
#include <string>

namespace wd{

class SpellcorrectServer{
public:
    SpellcorrectServer(const std::string &cfgFileName);
    void start();
    void onConnection(TcpConnectionPtr conn);
    void onMessage(TcpConnectionPtr conn);
    void onClose(TcpConnectionPtr conn);
private:
    MyConf _conf;
    TcpServer _tcpSever;
    ThreadPool _threadpool;
    TimerThread _timer;
};

}

#endif
