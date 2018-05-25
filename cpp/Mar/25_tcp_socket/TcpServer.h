#ifndef __MY_TCPSERVER_H__
#define __MY_TCPSERVER_H__

#include "EpollPoller.h"
#include "Socket.h"
#include "Nocopyable.h"

namespace wd{

class TcpServer : Noncopyable{
    typedef EpollPoller::EpollPollerCallback TcpServerCallback;
public:
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
    TcpServerCallback _onConnectionCb;
    TcpServerCallback _onMessageCb;
    TcpServerCallback _onCloseCb;

};

}//end of namespace wd

#endif
