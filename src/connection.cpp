#include "ipc/socket/connection.hpp"
#include "ipc/errno_except.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdexcept>

namespace ipc {

namespace socket {

connection::connection(fd_owner fd) : fd_(std::move(fd)) {}

const fd_owner& connection::file_descriptor() const noexcept {
    return fd_;
}

fd_owner& connection::file_descriptor() noexcept {
    return fd_;
}

ssize_t connection::recv(uint8_t *dest, std::size_t size) const {
    ssize_t bytes_read = ::read(fd_, dest, size);
    if (bytes_read == -1)
        throw errno_except(errno);
    return bytes_read;
}

ssize_t connection::send(const uint8_t *src, std::size_t size) const {
    ssize_t bytes_read = ::write(fd_, src, size);
    if (bytes_read == -1)
        throw errno_except(errno);
    return bytes_read;
}

connection connect(type tp, const address& addr) {
    fd_owner con_fd(::socket(AF_UNIX, to_sock(tp), 0));

    if (int(con_fd) == -1)
        throw errno_except(errno);

    int ret = ::connect(int(con_fd),
                        (const struct sockaddr *) &addr.as_unix(),
                        sizeof(struct sockaddr_un));

    if (ret == -1)
        throw errno_except(errno);

    return connection(std::move(con_fd));
}

} // namespace socket


} // namespace ipc
