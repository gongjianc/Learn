#ifndef __MY_SOCKET_H__
#define __MY_SOCKET_H__

#include "Nocopyable.h"

namespace wd{

class InetAddress;

class Socket : Noncopyable{
public:
    Socket();
    Socket(int fd);
    void ready(InetAddress &addr);
    int accept();
    int fd();
    //int listenfd();
    
    static InetAddress getLocalAddress(int sockfd);
    static InetAddress getPeerAddress(int sockdf);

    //服务器只关闭写端
    //TCP 是全双工的，有两端
    void shutdownWirte();
private:
    void bind(InetAddress &addr);
    int listen();
    void setReuseAddr(bool flag);
    void setReusePort(bool flag);

private:
    int _sockfd;
};

}//end of namespace wd



#endif
