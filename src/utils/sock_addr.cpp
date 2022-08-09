#include "sock_addr.h"
#include <arpa/inet.h>

#include <string.h>


SockAddr::SockAddr(std::string addr, size_t port)
{
    sa_.sin_family = AF_INET;
    sa_.sin_port = htons(port);
    inet_pton(AF_INET, addr.c_str(), &sa_.sin_addr);
}


SockAddr::SockAddr(Type addr, size_t port)
{
    sa_.sin_family = AF_INET;
    sa_.sin_port = htons(port);
    sa_.sin_addr.s_addr = htonl(static_cast<uint32_t>(addr));
}


std::string SockAddr::Str() const
{
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &sa_.sin_addr, buf, INET_ADDRSTRLEN);
    return buf;
}


size_t SockAddr::Port() const
{
    return ntohs(sa_.sin_port);
}


const struct sockaddr *SockAddr::Addr() const {
    return reinterpret_cast<const struct sockaddr *>(&sa_);
}


socklen_t SockAddr::AddrLen() const {
    return sizeof(sa_);
}
