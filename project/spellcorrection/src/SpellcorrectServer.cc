/*
** @file    SpellcorrectServer.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-18 17:51:50
*/

#include "SpellcorrectServer.h"
#include "MyDict.h"
#include "MyTask.h"
#include <iostream>
#include <sstream>
#include <functional>

unsigned short strToUnshort(const std::string &str)
{
    unsigned short port;
    std::stringstream ss;
    ss << str;
    ss >> port;
    return port;
}

namespace wd{
SpellcorrectServer::SpellcorrectServer(const std::string &cfgFileName)
    : _conf(cfgFileName)
    , _tcpSever(_conf.getMap()["my_ip"].c_str(), strToUnshort(_conf.getMap()["my_port"]))
    , _threadpool(4, 1000)
{
    MyDict *pMyDict = MyDict::createInstance();
    pMyDict->init(_conf.getMap()["my_dict"].c_str(), _conf.getMap()["my_cn_dict"].c_str());
}

void SpellcorrectServer::start()
{
    //启动线程池
    _threadpool.start();
    //这是？
    //占位
    using namespace std::placeholders;
    _tcpSever.setConnectionCallback(std::bind(&SpellcorrectServer::onConnection, this, _1));
    _tcpSever.setMessageCallback(std::bind(&SpellcorrectServer::onMessage, this, _1));
    _tcpSever.setCloseCallback(std::bind(&SpellcorrectServer::onClose, this, _1));
    _tcpSever.start();
}

void SpellcorrectServer::onConnection(TcpConnectionPtr conn)
{
    std::cout << conn->toString() << "has connected!" << std::endl;
    conn->send("welcome to SpellCorrect Server!");
}

void SpellcorrectServer::onMessage(TcpConnectionPtr conn)
{
    std::string msg = conn->receive();
    std::cout << msg << std::endl;
    MyTask task(msg, conn->fd());
    _threadpool.addTask(std::bind(&MyTask::execute, task));
}

void SpellcorrectServer::onClose(TcpConnectionPtr conn)
{
    std::cout << conn->toString() << "has closed!" << std::endl;
}

}


