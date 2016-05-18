#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Noncopyable.h"

namespace wd{

class InetAddress;

class Socket : Noncopyable{
public:
    Socket();
    Socket(int);
    void ready(InetAddress &addr);
    int accept();
    int fd();

    void shutdownWrite();
    static InetAddress getLocalAddr(int fd);
    static InetAddress getPeerAddr(int fd);
private:
    void setReuseAddr(bool flag);
    void setReusePort(bool flag);
    void bind(InetAddress &addr);
    void listen();
private:
    int _fd;
};

}//end of namespace

#endif
