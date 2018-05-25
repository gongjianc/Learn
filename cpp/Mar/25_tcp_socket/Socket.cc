/*
** @file    Socket.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-25 01:32:42
*/
 
#include "Socket.h"
#include "InetAddress.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>

int createSocketFd()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == fd){
        perror("socket error");
    }
    return fd;
}

namespace wd{

Socket::Socket()
: _sockfd(createSocketFd())
{}

Socket::Socket(int fd)
: _sockfd(fd)
{}

void Socket::ready(InetAddress &addr)
{
    setReuseAddr(true);
    setReusePort(true);
    bind(addr);
    listen();
}

int Socket::accept()
{
    int peerfd = ::accept(_sockfd, NULL, NULL);
    if(-1 == peerfd){
        perror("accpet error");
    }
    return peerfd;
}

void Socket::bind(InetAddress &addr)
{
    int ret = ::bind(_sockfd, addr.getInetAddressPtr(), sizeof(struct sockaddr));
    if(-1 == ret){
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

int Socket::listen()
{
    int ret = ::listen(_sockfd, 10);
    if(-1 == ret){
        perror("listen");
        close(_sockfd);
        exit(EXIT_FAILURE);
    }
    return ret;
}

int Socket::fd()
{
    return _sockfd;
}

InetAddress Socket::getLocalAddress(int sockfd)
{
   struct sockaddr_in addr;
   socklen_t len = sizeof(sockaddr_in);
   int ret = ::getsockname(sockfd, (struct sockaddr *)&addr, &len);
   if(-1 == ret)
       perror("getsockname");
   return InetAddress(addr);
}

InetAddress Socket::getPeerAddress(int sockfd)
{
   struct sockaddr_in addr;
   socklen_t len = sizeof(sockaddr_in);
   int ret = ::getpeername(sockfd, (struct sockaddr *)&addr, &len);
   if(-1 == ret)
       perror("getpeername");
   return InetAddress(addr);
}


void Socket::setReuseAddr(bool flag)
{
    int on = (flag ? 1 : 0);
    if(::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, 
                    static_cast<socklen_t>(sizeof(on))) == -1){
        perror("setReusseAddr");
        ::close(_sockfd);
        exit(EXIT_FAILURE);
    }
}

void Socket::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
    int on = (flag ? 1 : 0);
    //服务器负载均衡
    if(::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT, &on, 
                    static_cast<socklen_t>(sizeof(on))) == -1){
        perror("setReussePort");
        ::close(_sockfd);
        exit(EXIT_FAILURE);
    }
#else
    if(flag)
        fprintf(stderr, "SO_REUSEPORT is not supported!\n");

#endif
}

void Socket::shutdownWirte()
{
    int ret = ::shutdown(_sockfd, SHUT_WR);
    if(-1 == ret){
        perror("shundownWrite");
        close(_sockfd);
        exit(EXIT_FAILURE);
    }
}


}//end of namespace wd

