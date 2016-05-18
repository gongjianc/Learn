#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

#include "Noncopyable.h"
#include <unistd.h>

namespace wd{
class SocketIO : Noncopyable{
public:
    //为什么这么设计？
    SocketIO(int fd);

    ssize_t readn(char *buf, size_t count);
    ssize_t writen(const char *buf, size_t count);
    ssize_t readline(char *buf, size_t max);
private:
    //预读
    ssize_t readPeek(char *buf, size_t count);
private:
    int _fd;
};

}// end of namespace 

#endif
