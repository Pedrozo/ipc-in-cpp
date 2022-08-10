#ifndef IPC_SOCKET_CONNECTION_HPP_
#define IPC_SOCKET_CONNECTION_HPP_

#include <string>
#include <ios>
#include <memory>
#include "../fd_owner.hpp"
#include "type.hpp"
#include "address.hpp"

namespace ipc {

namespace socket {

class connection {
public:
    explicit connection(fd_owner fd);

    ~connection();

    const fd_owner& file_descriptor() const noexcept;

    fd_owner& file_descriptor() noexcept;

    std::istream& in() noexcept;

    std::ostream& out() noexcept;

private:
    struct impl;
    std::unique_ptr<impl> pimpl_;
};

connection connect(type tp, const address& addr);

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_CONNECTION_HPP_