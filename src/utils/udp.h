#pragma once


#include "sock_addr.h"
#include "packets.h"


namespace UDP {
    
    class Socket {
        
    public:
        Socket();
        ~Socket();

        void AllowBroadcast();
        bool HasBroadcast() const;
        int FD() const;
        void Close();
        bool Bind(SockAddr &sa);
        
    private:
        int fd_;
    };
    
    ssize_t SendTo(Socket &sock, SockAddr &sa, Packet &packet);
    ssize_t RecvFrom(Socket &sock, Packet &packet);
}

