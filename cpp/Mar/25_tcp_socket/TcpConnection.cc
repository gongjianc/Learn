/*
** @file    TcpConnection.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-25 08:09:01
*/
 
#include "TcpConnection.h"
#include "InetAddress.h"
#include <string.h>
#include <sstream>

namespace wd{

TcpConnection::TcpConnection(int fd)
: _sock(fd)
, _sockIO(fd)
, _localAddr(Socket::getLocalAddress(fd))
, _peerAddr(Socket::getPeerAddress(fd))
, _isShundowmWrite(false)
{}

std::string TcpConnection::receive()
{
    //栈里边存储的数据返回？
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    _sockIO.readline(buf, 1024);
    return std::string(buf);
}

void TcpConnection::send(const std::string &msg)
{
    _sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::shutdown()
{
    if(!_isShundowmWrite){
        _isShundowmWrite = true;
        _sock.shutdownWirte();
    }
}

TcpConnection::~TcpConnection()
{
    if(!_isShundowmWrite)
        shutdown();
}

//必须加const： 非const函数不可以访问const成员
std::string TcpConnection::toString() const 
{
    std::ostringstream oss;
    oss << _localAddr.ip() << ":" 
        << _localAddr.port() << " --> "
        << _peerAddr.ip() << ":"
        << _peerAddr.port(); 
    return oss.str();
}

void TcpConnection::setTcpConnectionCallback(TcpConnectionCallback cb)
{
    _onConnectionCb = cb;
}

void TcpConnection::handleTcpConnectionCallback()
{
    if(_onConnectionCb)
        _onConnectionCb(shared_from_this());
}

void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{
    _onMessageCb = cb;
}

void TcpConnection::handleMessageCallbak()
{
    //std::function定义的回调函数的参数是 shared_ptr<TcpConnection>---->TcpConnectionPtr
    //需要参数 
    if(_onMessageCb)
        _onMessageCb(shared_from_this());
}

void TcpConnection::setCloseMessageCallback(TcpConnectionCallback cb)
{
    _onCloseCb = cb;
}

void TcpConnection::handleCloseCallback()
{
    if(_onCloseCb)
        _onCloseCb(shared_from_this());
}

}//end of namespace wd
