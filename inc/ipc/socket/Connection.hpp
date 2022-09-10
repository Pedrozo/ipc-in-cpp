#ifndef IPC_SOCKET_CONNECTION_HPP
#define IPC_SOCKET_CONNECTION_HPP

#include "../FileDescriptor.hpp"
#include "../Utils.hpp"
#include "Address.hpp"
#include "Type.hpp"

namespace ipc {

namespace socket {

class Connection {
public:

    explicit Connection(FileDescriptor fd) : fd_(std::move(fd)) {}

    ssize_t Recv(void *data, std::size_t size) const {
        return CallSys(::read, int(fd_), data, size);
    }

    ssize_t Send(const void *data, std::size_t size) const {
        return CallSys(::write, int(fd_), data, size);
    }

private:
    FileDescriptor fd_;
};

inline Connection Connect(Type type, const Address& address) {
    int sock_type = type == Type::STREAM ? SOCK_STREAM : SOCK_SEQPACKET;
    FileDescriptor fd(CallSys(::socket, AF_UNIX, sock_type, 0));

    CallSys(::connect, int(fd), (const struct sockaddr *) &address.UnixAddress(), address.Length());

    return Connection(std::move(fd));
}

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_CONNECTION_HPP