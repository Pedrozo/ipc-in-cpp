#include "ipc/errno_except.hpp"

#include <sys/un.h>

namespace ipc {

errno_except::errno_except(int error_number) : error_number_(error_number) {}

int errno_except::error_number() const noexcept {
    return error_number_;
}

const char *errno_except::what() const noexcept {
    return strerror(error_number_);
}

} // namespace ipc
