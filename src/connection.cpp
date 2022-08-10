#include "ipc/socket/connection.hpp"
#include "ipc/errno_except.hpp"
#include "ipc/fd_device.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <stdexcept>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/device/file.hpp>

namespace ipc {

namespace socket {

struct connection::impl {
    explicit impl(fd_owner f)
        : fd(std::move(f)), stream_buffer(fd_device(fd)),
          in_stream(&stream_buffer), out_stream(&stream_buffer) {}

    ~impl() = default;

    impl(const impl&) = delete;

    impl(impl&&) = delete;

    impl& operator=(const impl&) = delete;

    impl& operator=(impl&&) = delete;

    fd_owner fd;
    boost::iostreams::stream_buffer<fd_device> stream_buffer;
    std::istream in_stream;
    std::ostream out_stream;
};
    
connection::connection(fd_owner fd) : pimpl_(std::make_unique<connection::impl>(std::move(fd))) {}

connection::~connection() = default;

const fd_owner& connection::file_descriptor() const noexcept {
    return pimpl_->fd;
}

fd_owner& connection::file_descriptor() noexcept {
    return pimpl_->fd;
}

std::istream& connection::in() noexcept {
    return pimpl_->in_stream;
}

std::ostream& connection::out() noexcept {
    return pimpl_->out_stream;
}

connection connect(type tp, const address& addr) {
    fd_owner con_fd(::socket(AF_UNIX, to_sock(tp), 0));

    if (int(con_fd) == -1)
        throw errno_except(errno);

    struct sockaddr_un socket_address;

    memset(&socket_address, 0, sizeof(struct sockaddr_un));
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, addr.str(), sizeof(socket_address.sun_path) - 1);

    int ret = ::connect(int(con_fd),
                        (const struct sockaddr *) &socket_address,
                        sizeof(struct sockaddr_un));

    if (ret == -1)
        throw errno_except(errno);

    return connection(std::move(con_fd));
}

// connection connect(const std::string& address) {
    
// }

} // namespace socket


} // namespace ipc
