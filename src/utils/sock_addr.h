#pragma once

#include <string>
#include <netinet/in.h>

class SockAddr {
    
public:
    enum class Type : uint32_t {
        ANY = INADDR_ANY,
        BROADCAST = INADDR_BROADCAST
    };
    
    SockAddr(std::string addr, size_t port);
    SockAddr(Type addr, size_t port);
    
    std::string Str() const;
    size_t Port() const;
    
    const struct sockaddr *Addr() const;
    socklen_t AddrLen() const;
    
private:
    struct sockaddr_in sa_;
    
};
