#ifndef IPC_SOCKET_SERVER_HPP_
#define IPC_SOCKET_SERVER_HPP_

#include <string>
#include "../fd_owner.hpp"
#include "connection.hpp"

namespace ipc {

namespace socket {

class server {
public:
    explicit server(fd_owner fd);

    const fd_owner& file_descriptor() const noexcept;

    fd_owner& file_descriptor() noexcept;

    connection accept() const;

private:
    fd_owner fd_;
};

server make_server(type tp, const address& addr, int backlog);

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_SERVER_HPP_