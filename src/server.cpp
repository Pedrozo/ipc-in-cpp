#include "ipc/socket/server.hpp"
#include "ipc/errno_except.hpp"

#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>

namespace ipc {
    
namespace socket {

server::server(fd_owner fd) : fd_(std::move(fd)) {}

const fd_owner& server::file_descriptor() const noexcept {
    return fd_;
}

fd_owner& server::file_descriptor() noexcept {
    return fd_;
}

connection server::accept() const {
    int con_fd = ::accept(int(fd_), nullptr, nullptr);

    if (con_fd == -1)
        throw errno_except(errno);

    return connection(fd_owner(con_fd));
}

server make_server(type tp, const address& addr, int backlog) {
    struct sockaddr_un sock_addr;
    fd_owner sock_fd(::socket(AF_UNIX, to_sock(tp), 0));

    if (int(sock_fd) == -1)
        throw errno_except(errno);

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sun_family = AF_UNIX;
    strncpy(sock_addr.sun_path, addr.str(), sizeof(sock_addr.sun_path) - 1);

    int ret = bind(int(sock_fd), (const struct sockaddr *) &sock_addr, sizeof(sock_addr));
    if (ret == -1)
        throw errno_except(errno);

    ret = listen(int(sock_fd), backlog);
    if (ret == -1)
        throw errno_except(errno);

    return server(std::move(sock_fd));
}

} // namespace socket

} // namespace ipc
