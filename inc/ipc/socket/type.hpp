#ifndef IPC_SOCKET_TYPE_HPP_
#define IPC_SOCKET_TYPE_HPP_

namespace ipc {

namespace socket {

enum class type {
    STREAM,
    DGRAM,
    SEQPACKET
};

int to_sock(type tp);

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_TYPE_HPP_