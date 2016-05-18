#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Noncopyable.h"
#include "Socket.h"
#include "EpollPoller.h"

namespace wd{

class TcpServer : Noncopyable{
public:
    typedef EpollPoller::EpollPollerCallback TcpServerCallback;
    TcpServer(unsigned short port);
    TcpServer(const char *ip, unsigned short port);

    void start();
    void stop();
    void setConnectionCallback(TcpServerCallback cb);
    void setMessageCallback(TcpServerCallback cb);
    void setCloseCallback(TcpServerCallback cb);
private:
    Socket _sock;
    EpollPoller _epoll;

    TcpServerCallback _onConnection;
    TcpServerCallback _onMessage;
    TcpServerCallback _onClose;
}; 

}

#endif

