#include "ipc/socket/address.hpp"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>

namespace ipc {

namespace socket {

address::address(const char *str) {
    std::size_t len = strnlen(str, MAX_LEN + 1);

    if (len == MAX_LEN + 1)
        throw std::invalid_argument("Given address length is greater than the address limit");

    strncpy(str_, str, MAX_LEN + 1);
}

address::address(const std::string& str) : address(str.c_str()) {}

const char *address::str() const noexcept {
    return str_;
}

} // namespace socket

} // namespace ipc
