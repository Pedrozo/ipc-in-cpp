#ifndef IPC_SOCKET_SERVER_HPP
#define IPC_SOCKET_SERVER_HPP

#include "Connection.hpp"

namespace ipc {

namespace socket {

class Server {
public:

    explicit Server(FileDescriptor fd) : fd_(std::move(fd)) {}

    Connection Accept() const {
        return Connection(FileDescriptor(CallSys(::accept, int(fd_), nullptr, nullptr)));
    }

private:
    FileDescriptor fd_;
};

inline Server MakeServer(Type type, const Address& address, int backlog) {
    int sock_type = type == Type::STREAM ? SOCK_STREAM : SOCK_SEQPACKET;
    FileDescriptor fd(CallSys(::socket, AF_UNIX, sock_type, 0));

    CallSys(::bind, int(fd), (const struct sockaddr *) &address.UnixAddress(), address.Length());
    CallSys(::listen, int(fd), backlog);

    return Server(std::move(fd));
}

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_SERVER_HPP