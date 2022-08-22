#ifndef IPC_SOCKET_CONNECTION_HPP_
#define IPC_SOCKET_CONNECTION_HPP_

#include <string>
#include <ios>
#include <memory>
#include <sys/types.h>
#include "../fd_owner.hpp"
#include "type.hpp"
#include "address.hpp"

namespace ipc {

namespace socket {

class connection {
public:
    explicit connection(fd_owner fd);

    const fd_owner& file_descriptor() const noexcept;

    fd_owner& file_descriptor() noexcept;

    ssize_t recv(uint8_t *dest, std::size_t size) const;

    ssize_t send(const uint8_t *src, std::size_t size) const;

private:
    fd_owner fd_;
};

connection connect(type tp, const address& addr);

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_CONNECTION_HPP_