#ifndef __MY_INETADDRESS_H__
#define __MY_INETADDRESS_H__

#include <arpa/inet.h>
#include <string>

namespace wd{

class InetAddress{
public:
    InetAddress(unsigned short port);
    InetAddress(const char *ip, unsigned short port);
    InetAddress(const struct sockaddr_in &add);

    std::string ip() const;
    unsigned short port() const;
    struct sockaddr *getInetAddressPtr();

private:
    struct sockaddr_in _addr;
};
}//end of namespace



#endif
