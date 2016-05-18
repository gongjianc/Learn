#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <netinet/in.h>
#include <string>

namespace wd{
class InetAddress{
public:
    InetAddress(unsigned short port);
    InetAddress(const char *ip, unsigned short port);
    InetAddress(const struct sockaddr_in &addr);

    //?
    struct sockaddr_in *getInetAddressPtr();
    
    std::string ip() const;
    unsigned short port() const;
private:
    struct sockaddr_in _addr;
};
}//end 

#endif
