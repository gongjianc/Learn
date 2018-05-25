/*
** @file    TcpServer.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-26 08:50:14
*/
 
#include "TcpServer.h"
#include "Socket.h"
#include "InetAddress.h"

namespace wd{

TcpServer::TcpServer(unsigned short port)
: _sock()
, _epoll(_sock.fd())
{
    InetAddress addr(port);
    _sock.ready(addr);
}

TcpServer::TcpServer(const char *ip, unsigned short port)
: _sock()
, _epoll(_sock.fd())
{
    InetAddress addr(ip, port);
    _sock.ready(addr);
}

void TcpServer::start()
{
    _epoll.setTcpConnectionCallback(_onConnectionCb);
    _epoll.setMessageCallback(_onMessageCb);
    _epoll.setCloseCallback(_onCloseCb);
    _epoll.loop();
}

void TcpServer::stop()
{
    _epoll.unloop();
}

void TcpServer::setConnectionCallback(TcpServerCallback cb)
{
    _onConnectionCb = cb;
}

void TcpServer::setMessageCallback(TcpServerCallback cb)
{
    _onMessageCb = cb;
}

void TcpServer::setCloseCallback(TcpServerCallback cb)
{
    _onCloseCb = cb;
}

}//end of namespace wd
