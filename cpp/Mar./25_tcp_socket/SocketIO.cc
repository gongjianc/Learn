/*
** @file    SocketIO.cc
** @author  jaygong(gongjian0309@gmail.com)
** @date    2016-03-24 06:16:37
*/
 
#include "SocketIO.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>

namespace wd{

SocketIO::SocketIO(int sockfd)
: _sockfd(sockfd)
{}

ssize_t SocketIO::readn(char *buf, size_t count)
{
    size_t nleft = count;
    char *pbuf = buf;
    while(nleft > 0){
        ssize_t nread = read(_sockfd, pbuf, nleft);
        if(nread == -1){
            if(errno == EINTR)
                continue;
            perror("readn");
            exit(EXIT_FAILURE);
        }else if(nread == 0){
            break;//读到文件末尾
        }
        nleft -= nread;
        pbuf += nread;
    }
    return count - nleft; //
}

ssize_t  SocketIO::writen(const char *buf, size_t count)
{
    size_t nleft = count;
    const char *pbuf = buf;
    while(nleft > 0){
        ssize_t nwrite = write(_sockfd, pbuf, nleft);
        if(nwrite == -1){
            if(errno == EINTR)
                continue;
            perror("writen");
            exit(EXIT_FAILURE);
        }else if(0 == nwrite)
            break;
        nleft -= nwrite;
        pbuf += nwrite;
    }
    return count - nleft;
}

ssize_t SocketIO::readline(char *buf, size_t maxLen)
{
    size_t nleft = maxLen - 1;//如果maxLen没有'\n',就取maxLen - 1 个字节
    char *pbuf = buf;
    ssize_t total = 0;
    while(nleft > 0){
        ssize_t nread = recv_peek(pbuf, nleft);
        if(nread < 0)
            return nread;
        for(ssize_t idx = 0; idx != nread; ++idx){
            if(pbuf[idx] == '\n'){
                ssize_t nsize = idx + 1;//为什么+1
                if(readn(pbuf, nsize) != nsize)
                    return EXIT_FAILURE;
                nleft -= nsize;
                total += nsize;
                pbuf += nsize;
                //换行
                *pbuf = '\0';
                return total;
            }
        }
        //没有找到，移走内核缓冲区的数据
        if(readn(pbuf, nread) != nread)
            return EXIT_FAILURE;
        nleft -= nread;
        pbuf += nread;
        total += nread;
    }
    *pbuf = '\0';
    return maxLen - 1;
}

ssize_t SocketIO::recv_peek(char *buf, size_t count)
{
    ssize_t nread;
    do{
        nread = ::recv(_sockfd, buf, count, MSG_PEEK);
    }while(nread == -1 && errno == EINTR);
    return nread;
}
}// end of namespace w
