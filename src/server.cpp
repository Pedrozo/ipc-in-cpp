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
    fd_owner sock_fd(::socket(AF_UNIX, to_sock(tp), 0));

    if (int(sock_fd) == -1)
        throw errno_except(errno);

    int ret = bind(sock_fd, (const struct sockaddr *) &addr.as_unix(), sizeof(struct sockaddr_un));
    if (ret == -1)
        throw errno_except(errno);

    ret = listen(int(sock_fd), backlog);
    if (ret == -1)
        throw errno_except(errno);

    return server(std::move(sock_fd));
}

} // namespace socket

} // namespace ipc
