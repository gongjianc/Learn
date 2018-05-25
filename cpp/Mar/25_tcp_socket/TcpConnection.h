/*
 * 简化发送和接受msg的
 */
#ifndef __MY_TCPCONNECTION_H__
#define __MY_TCPCONNECTION_H__

#include <string>
#include "SocketIO.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Nocopyable.h"
#include <functional>
#include <memory>

namespace wd{

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection
: Noncopyable
, public std::enable_shared_from_this<TcpConnection>{
public:
    typedef std::function<void(TcpConnectionPtr)> TcpConnectionCallback;
    TcpConnection(int fd);
    ~TcpConnection();
    std::string receive();
    void send(const std::string &msg);
    void shutdown();
    std::string toString() const;
    void setTcpConnectionCallback(TcpConnectionCallback cb);
    void handleTcpConnectionCallback();
    void setMessageCallback(TcpConnectionCallback cb);
    void handleMessageCallbak();
    void setCloseMessageCallback(TcpConnectionCallback cb);
    void handleCloseCallback();

private:
    Socket _sock;
    SocketIO _sockIO;
    //const 连接建立后，本地和对端的地址是不能修改的
    const InetAddress _localAddr;
    const InetAddress _peerAddr;
    bool _isShundowmWrite;
    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

}//end of namespace wd

#endif
