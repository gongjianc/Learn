/*
** @file    TcpConnection.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 22:05:07
*/
 
#include "TcpConnection.h"
#include <string.h>
#include <iostream>
#include <sstream>

namespace wd{
TcpConnection::TcpConnection(int fd)
    : _sock(fd)
    , _sockIO(fd)
    , _localAddr(Socket::getLocalAddr(fd))
    , _peerAddr(Socket::getPeerAddr(fd))
    , _isShutdownWrite(false)
    {}

TcpConnection::~TcpConnection()
{
    if(!_isShutdownWrite)
        shutdown();
}

std::string TcpConnection::receive()
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    _sockIO.readline(buf, 1024);
    return std::string(buf);
}

void TcpConnection::send(const std::string &msg)
{
    //std::cout << "TcpConnection::send" << std::endl;
    _sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::shutdown()
{
    if(!_isShutdownWrite){
        _isShutdownWrite = true;
        _sock.shutdownWrite();
    }
}

std::string TcpConnection::toString() const
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port()
        << "--->" << _peerAddr.ip() << ":" 
        << _peerAddr.port() << " ";
    return oss.str();
}

int TcpConnection::fd()
{
    return _sock.fd();
}

void TcpConnection::setConnectionCallback(TcpConnectionCallback cb)
{
    _onConnectionCb = cb;
}

void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{
    _onMessageCb = cb;
}

void TcpConnection::setCloseCallback(TcpConnectionCallback cb)
{
    _onCloseCb = cb;
}

void TcpConnection::handleConnectionCallback()
{
    if(_onConnectionCb){
        //这个是？
        _onConnectionCb(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessageCb)
        _onMessageCb(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb)
        _onCloseCb(shared_from_this());
}

}




    
