#ifndef __MY_SOCKETIO_H__
#define __MY_SOCKETIO_H__

#include <stddef.h>
#include "Nocopyable.h"
#include <sys/types.h>
namespace wd{

class SocketIO : Noncopyable{
public:
    SocketIO(int sockfd);
    ssize_t readn(char *buf, size_t count);
    ssize_t writen(const char *buf, size_t count);
    ssize_t readline(char *buf, size_t maxLen);
private:
    ssize_t recv_peek(char *buf, size_t count);
private:
    int _sockfd;
};

}//end of namespace wd

#endif
