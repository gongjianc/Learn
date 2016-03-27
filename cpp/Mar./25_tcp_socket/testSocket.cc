/*
** @file    testSocket.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-25 02:15:37
*/
 
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include "TcpServer.h"
 
using std::cout;
using std::endl;

void onConnection(wd::TcpConnectionPtr pConn)
{
    cout << pConn->toString() << endl;
    pConn->send("welcome to server.");
}

//数据发送的时候
void onMessage(wd::TcpConnectionPtr pConn)
{
    std::string msg = pConn->receive();
    pConn->send("@server : " + msg);
}

void onClose(wd::TcpConnectionPtr pConn)
{
    cout << pConn->toString() 
         << " has closed. " << endl;
}

int main(void)
{
    wd::TcpServer tcpServer(8888);
    tcpServer.setConnectionCallback(onConnection);
    tcpServer.setMessageCallback(onMessage);
    tcpServer.setCloseCallback(onClose);
    tcpServer.start();
    tcpServer.stop();//不用线程没法关闭

#if 0
    wd::InetAddress addr(8888);
    wd::Socket sock;
    sock.ready(addr);

    //listenfd fd 方法没有
    wd::EpollPoller epollPoller(sock.fd());
    epollPoller.setTcpConnectionCallback(onConnection);
    epollPoller.setMessageCallback(onMessage);
    epollPoller.setCloseCallback(onClose);
    epollPoller.loop();
#endif 

#if 0
    int peerfd = sock.accept();
    char buf[1024] = "welcome to server!";

    wd::TcpConnection conn(peerfd);
    cout << conn.toString() << endl;
    conn.send(std::string(buf));
    while(1){
        std::string str = conn.receive();
        conn.send(str);
    }
#endif

#if 0
    printf("%s:%d --> %s:%d\n",
            wd::Socket::getLocalAddress(peerfd).ip().c_str(), 
            wd::Socket::getLocalAddress(peerfd).port(),
            wd::Socket::getPeerAddress(peerfd).ip().c_str(),
            wd::Socket::getPeerAddress(peerfd).port()
          );
#endif

#if 0
    write(peerfd, buf, strlen(buf));
    while(true){
        memset(buf, 0, sizeof(buf));
        read(peerfd, buf, sizeof(buf));
        write(peerfd, buf, strlen(buf));
    }
#endif
#if 0
    wd::SocketIO sockIO(peerfd);
    sockIO.writen(buf,strlen(buf));
    while(1){
        memset(buf, 0, sizeof(buf));
        //sockIO.readn(buf,strlen(buf));//为什么 readn会阻塞
        sockIO.readline(buf, sizeof(buf));
        sockIO.writen(buf, strlen(buf));
    }
#endif
    return 0;
}
