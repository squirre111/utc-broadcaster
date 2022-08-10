#include "udp.h"

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

#define MAX_UDP_SIZE 1500


UDP::Socket::Socket() : fd_(0)
{
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);
}


void UDP::Socket::AllowBroadcast()
{
    const int on = 1;
    setsockopt(fd_, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
}


bool UDP::Socket::HasBroadcast() const
{
    socklen_t optlen;
    int on;
    int rv = getsockopt(fd_, SOL_SOCKET, SO_BROADCAST, &on, &optlen);
    if (rv != 0)
        return false;
    return on;
}


int UDP::Socket::FD() const
{
    return fd_;
}


void UDP::Socket::Close() {
    close(fd_);
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
