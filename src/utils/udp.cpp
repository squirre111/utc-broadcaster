#include "udp.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

#define MAX_UDP_SIZE 1500


#include <iostream>

UDP::Socket::Socket() : fd_(0)
{
    fd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}


void UDP::Socket::AllowBroadcast()
{
    SetSockOption(SO_BROADCAST, 1);
}


void UDP::Socket::AllowReusePort()
{
    SetSockOption(SO_REUSEPORT, 1);
}


bool UDP::Socket::HasBroadcast() const
{
    int val;
    auto res = GetSockOption(SO_BROADCAST, &val);
    if (!res) return false;
    return val;
}


bool UDP::Socket::HasReusePort() const
{
    int val;
    auto res = GetSockOption(SO_REUSEPORT, &val);
    if (!res) return false;
    return val;
}


template<typename T>
void UDP::Socket::SetSockOption(int optname, const T val) {
    setsockopt(fd_, SOL_SOCKET, optname, &val, sizeof(T));
}


template<typename T>
bool UDP::Socket::GetSockOption(int optname, T *val) const {
    socklen_t optlen;
    int rv = getsockopt(fd_, SOL_SOCKET, optname, val, &optlen);
    return sizeof(T) == optlen && rv == 0;
}


int UDP::Socket::FD() const
{
    return fd_;
}


void UDP::Socket::Close() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}


bool UDP::Socket::Bind(SockAddr &sa)
{
    return bind(fd_, sa.Addr(), sa.AddrLen()) == 0;
}


UDP::Socket::~Socket()
{
    Close();
}


ssize_t UDP::SendTo(const Socket &socket, const SockAddr &sa, Packet *packet)
{
    int flags = 0;
    return sendto(socket.FD(), packet->Data(), packet->Size(), flags, sa.Addr(), sa.AddrLen());
}


ssize_t UDP::RecvFrom(const Socket &socket, Packet *packet)
{
    int flags = 0;
    static char mesg[MAX_UDP_SIZE];
    ssize_t n = recvfrom(socket.FD(), mesg, MAX_UDP_SIZE, flags, NULL, NULL);
    packet->SetData(mesg, n);
    return n;
}
