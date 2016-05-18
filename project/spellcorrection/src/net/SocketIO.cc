/*
** @file    SocketIO.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-05-17 20:47:21
*/
 
#include "SocketIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace wd{

SocketIO::SocketIO(int fd)
    :_fd(fd)
{}

ssize_t SocketIO::readn(char *buf, size_t count)
{
    size_t nleft = count;
    char *pBuf = buf;
    ssize_t ret = 0;
    while(nleft > 0){
        ret = read(_fd, pBuf, nleft);
        if(-1 == ret){
            if(errno == EINTR)
                continue;
            perror("read");
            exit(EXIT_FAILURE);
        }else if(0 == ret){
            break;
        }
        pBuf += ret;
        nleft -= ret;
    }
    return (count - nleft);
}

ssize_t SocketIO::writen(const char *buf, size_t count)
{
    size_t nleft = count;
    const char *pBuf = buf;
    ssize_t ret = 0;
    while(nleft > 0){
        ret = write(_fd, pBuf, nleft);
        if(-1 == ret){
            if(errno == EINTR)
                continue;
            perror("write");
            exit(EXIT_FAILURE);
        }
        pBuf += ret;
        nleft -= ret;
    }
    return (count - nleft);
}

ssize_t SocketIO::readPeek(char *buf, size_t count)
{
    ssize_t nread;
    do{
        nread = ::recv(_fd, buf, count, MSG_PEEK);
    }while(nread == -1 && errno == EINTR);
    return nread;
}

ssize_t SocketIO::readline(char *buf, size_t max)
{
    size_t nleft = max - 1;

    char *pBuf = buf;
    size_t total = 0;
    while(nleft > 0){
        ssize_t nread = readPeek(pBuf, nleft);

        if(nread < 0)
            return nread;

        for(int idx = 0; idx != nread; ++idx){
            if(pBuf[idx] == '\n'){
                int nsize = idx + 1;
                if(readn(pBuf, nsize) != nsize)
                    return EXIT_FAILURE;
                nleft -= nsize;
                total += nsize;
                pBuf += nsize;
                *pBuf = '\0';
                return total;
            }
        }

        //没有找到，就移走内核缓冲区里面的 nread 个字节的数据
        if(readn(pBuf, nread) != nread)
            return EXIT_FAILURE;

        nleft -= nread;
        pBuf += nread;
        total += nread;
    }
    *pBuf = '\0';
    return max - 1;
}




}
