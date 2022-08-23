#include "ipc/socket/address.hpp"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifdef __ANDROID__
#define OFFSET 1
#else
#define OFFSET 0
#endif

namespace ipc {

namespace socket {

const std::size_t address::MAX_LEN =
    sizeof((struct sockaddr_un*)0)->sun_path / sizeof(char) - 1 - OFFSET;

address::address(const char *str) : addr_() {
    std::size_t len = strnlen(str, MAX_LEN + 1);

    if (len == MAX_LEN - OFFSET + 1)
        throw std::invalid_argument("Given address length is greater than the address limit");

    std::memset(&addr_, 0, sizeof(struct sockaddr_un));
    addr_.sun_family = AF_UNIX;
    strncpy(addr_.sun_path + OFFSET, str, sizeof(addr_.sun_path) - 1 - OFFSET);
}

address::address(const std::string& str) : address(str.c_str()) {}

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

} // namespace socket

} // namespace ipc
