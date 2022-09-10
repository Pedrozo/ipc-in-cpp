#ifndef IPC_SOCKET_ENDPOINT_HPP
#define IPC_SOCKET_ENDPOINT_HPP

#include "../Utils.hpp"
#include "../FileDescriptor.hpp"
#include "Address.hpp"

namespace ipc {

namespace socket {

class Endpoint {
public:

    explicit Endpoint(FileDescriptor fd) : fd_(std::move(fd)) {}

    explicit Endpoint(const Address& addr) : fd_(CallSys(::socket, AF_UNIX, SOCK_DGRAM, 0)) {
        CallSys(::bind, int(fd_), (const sockaddr *) &addr.UnixAddress(), addr.Length());
    }

    ssize_t Recv(void *data, std::size_t size) const {
        return CallSys(::recvfrom, int(fd_), data, size, 0, nullptr, nullptr);
    }

    ssize_t Recv(void *data, std::size_t size, Address& from) const {
        return CallSys(::recvfrom, int(fd_), data, size, 0, (sockaddr *) &from.UnixAddress(), &from.Length());
    }

    ssize_t Send(const void *data, std::size_t size, const Address& to) const {
        return CallSys(::sendto, int(fd_), data, size, 0, (const sockaddr *) &to.UnixAddress(), to.Length());
    }

private:
    FileDescriptor fd_;
};

inline Endpoint MakeEndpoint(const Address& address) {
    FileDescriptor fd(CallSys(::socket, AF_UNIX, SOCK_DGRAM, 0));
    CallSys(::bind, int(fd), (const sockaddr *) &address.UnixAddress(), address.Length());
    return Endpoint(std::move(fd));
}

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_ENDPOINT_HPP