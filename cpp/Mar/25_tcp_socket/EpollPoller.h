#ifndef __MY_EPOLLPOLLER_H__
#define __MY_EPOLLPOLLER_H__

#include "Nocopyable.h"
#include <vector>
#include <map>
#include "TcpConnection.h"
#include <sys/epoll.h>

namespace wd{

class EpollPoller : Noncopyable{
public:
    typedef TcpConnection::TcpConnectionCallback EpollPollerCallback;
    EpollPoller(int listenfd);
    ~EpollPoller();
    void loop();
    void unloop();
    void setTcpConnectionCallback(EpollPollerCallback cb);
    void setMessageCallback(EpollPollerCallback cb);
    void setCloseCallback(EpollPollerCallback cb);
private:
    void waitEpollfd();
    //处理新连接
    void handleConnection();
    void handleMessage(int fd);
private:
    int _epollfd;//epoll会占用一个文件描述符
    //_listenfd表示epoll去监听地fd
    //也就是服务器的socketfd
    //服务器accept之后，才会产生peerfd
    int _listenfd;
    bool _isLooping;

    std::vector<struct epoll_event> _eventList;

    std::map<int, TcpConnectionPtr> _connMap;

    EpollPollerCallback _onConnectionCb;
    EpollPollerCallback _onMessageCb;
    EpollPollerCallback _onCloseCb;
};
}//end of namespace wd

#endif
