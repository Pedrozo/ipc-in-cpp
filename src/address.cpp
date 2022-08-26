#include "ipc/socket/address.hpp"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace ipc {

namespace socket {

const std::size_t address::MAX_LEN = sizeof((struct sockaddr_un*)0)->sun_path / sizeof(char) - 1;

static void set_sock_addr(struct sockaddr_un& addr, const char *str, int offset) {
    std::size_t len = strnlen(str, address::MAX_LEN + 1);

    if (len >= address::MAX_LEN + 1 - offset)
        throw std::invalid_argument("Given address length is greater than the address limit");

    std::memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path + offset, str, sizeof(addr.sun_path) - 1 - offset);
}

const char *address::as_str() const noexcept {
    return addr_.sun_path;
}

const struct sockaddr_un& address::as_unix() const noexcept {
    return addr_;
}

void address::swap(address& other) noexcept {
    using std::swap;

    swap(addr_.sun_family, other.addr_.sun_family);

    for (int i = 0; i < sizeof(addr_.sun_path); i++)
        swap(addr_.sun_path[i], other.addr_.sun_path[i]);
}

void swap(address& a1, address& a2) noexcept {
    a1.swap(a2);
}

address make_path_address(const char *file_path) {
    address addr;
    set_sock_addr(addr.addr_, file_path, 0);
    return addr;
}

address make_abstract_address(const char *name) {
    address addr;
    set_sock_addr(addr.addr_, name, 1);
    return addr;
}

} // namespace socket

} // namespace ipc
