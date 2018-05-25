/*
** @file    EpollPoller.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-26 01:30:45
*/
 
#include "EpollPoller.h"
//#include <sys/epoll.h>//epoll_create1
#include <stdio.h>//perror
#include <stdlib.h>//EXIT_FAILURE 
#include <errno.h>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>

const int kInitNumber = 2048;

int createEpollFd()
{
    int fd = ::epoll_create1(0);
    if(-1 == fd){
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void addEpollFd(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = ::epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    if(-1 == ret){
        perror("epoll_ctl add");
        exit(EXIT_FAILURE);
    }
}

size_t recvPeek(int sockfd, char *buf, size_t count)
{
    int nread;
    do{
        nread = ::recv(sockfd, buf, count, MSG_PEEK);
    }while(-1 == nread && errno == EINTR);
    return nread;
}

bool isConnectionClosed(int sockfd)
{
    char buf[1024];
    int nread = recvPeek(sockfd, buf, 1024);
    if(-1 == nread){
        perror("recvPeek");
        exit(EXIT_FAILURE);
    }
    //nread == 0 则连接已经断开
    return (0 == nread);
}

// 是这样么？
void delEpollFd(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = ::epoll_ctl(epollfd, EPOLL_CTL_DEL, fd,  &ev);
    if(-1 == ret){
        perror("epoll_ctl add");
        exit(EXIT_FAILURE);
    }
}

/*----------------------------------------------------*/

namespace wd{

EpollPoller::EpollPoller(int listenfd)
: _epollfd(createEpollFd())
, _listenfd(listenfd)
, _isLooping(false)
, _eventList(kInitNumber)
{
    //_eventList.reserve(kInitNumber);
    addEpollFd(_epollfd, _listenfd);//为什么这里？
        
}

EpollPoller::~EpollPoller()
{
    ::close(_epollfd);
}

void EpollPoller::loop()
{
    if(!_isLooping){
        _isLooping = true;
        while(_isLooping){
            waitEpollfd();
        }
    }
}

void EpollPoller::unloop()
{
    if(_isLooping)
        _isLooping = false;
}

void EpollPoller::waitEpollfd()
{
    int nready;
    do{
        //阻塞
        nready = ::epoll_wait(_epollfd, &(*_eventList.begin()), 
                              _eventList.size(), 5000);
    }while(nready == -1 && errno == EINTR);

    if(-1 == nready){
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }else if(0 == nready){
        printf("epoll_wait timeout!\n");
    }else{
        if(nready == static_cast<int>(_eventList.size()))
            _eventList.resize(nready * 2);

        for(int idx = 0; idx != nready; ++idx){
            if(_eventList[idx].data.fd == _listenfd){
                if(_eventList[idx].events & EPOLLIN)
                    handleConnection();
            }else{
                if(_eventList[idx].events & EPOLLIN)
                    handleMessage(_eventList[idx].data.fd);
            }
        }
    }
}

//有新连接来的时候
void EpollPoller::handleConnection()
{
    int peerfd = ::accept(_listenfd, NULL, NULL);
    if(-1 == peerfd){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    //将新连接添加到epoll
    addEpollFd(_epollfd, peerfd);
   //堆对象是用户自己管理的，所以用指针
   //可以创建
   //裸指针不好管理，所以用shared_ptr
    TcpConnectionPtr pConn(new TcpConnection(peerfd));
    pConn->setTcpConnectionCallback(_onConnectionCb);
    pConn->setMessageCallback(_onMessageCb);
    pConn->setCloseMessageCallback(_onCloseCb);

    //添加pConn
    _connMap[peerfd] = pConn;

    pConn->handleTcpConnectionCallback();
    

    //给客户端发一些信息，当新连接发生的时候
    //要求可定制
    //回调函数
    //pConn->send("welcome to server.");
}

void EpollPoller::handleMessage(int fd)
{
    bool isClosed = isConnectionClosed(fd);
    std::map<int, TcpConnectionPtr>::iterator it = _connMap.find(fd);
    assert(it != _connMap.end());

    if(isClosed){
        //发现连接断开
        it->second->handleCloseCallback();
        delEpollFd(_epollfd, fd);
        _connMap.erase(it);
    }else{
        //客户端发来数据
        //
        it->second->handleMessageCallbak();
    }
}

void EpollPoller::setTcpConnectionCallback(EpollPollerCallback cb)
{
    _onConnectionCb = cb;
}

void EpollPoller::setMessageCallback(EpollPollerCallback cb)
{
    _onMessageCb = cb;
}

void EpollPoller::setCloseCallback(EpollPollerCallback cb)
{
    _onCloseCb = cb;
}

}//end of namespace wd
