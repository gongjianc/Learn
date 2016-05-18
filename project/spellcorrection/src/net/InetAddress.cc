/*
** @file    InetAddress.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 17:05:43
*/
 
#include "InetAddress.h"
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace wd{

InetAddress::InetAddress(unsigned short port)
{
    memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    //INADDR_ANY 是什么？
    _addr.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(const char *ip, unsigned short port)
{
    memset(&_addr, 0, sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip);
}

struct sockaddr_in *InetAddress::getInetAddressPtr()
{
    //返回的是指针
    return &_addr;
}

std::string InetAddress::ip() const
{
    return std::string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);
}

}

