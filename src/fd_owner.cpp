#include <algorithm>
#include <unistd.h>
#include "ipc/fd_owner.hpp"
#include "ipc/errno_except.hpp"

namespace ipc {

fd_owner::fd_owner(int fd) : fd_(fd) {}

fd_owner::~fd_owner() {
    if (fd_ != -1)
        ::close(fd_);
}

fd_owner::fd_owner(fd_owner&& other) noexcept : fd_(other.fd_) {
    other.fd_ = -1;
}

fd_owner& fd_owner::operator=(fd_owner&& other) noexcept {
    std::swap(fd_, other.fd_);
    return *this;
}

void fd_owner::close() {
    if (::close(fd_) == -1)
        throw errno_except(errno);
}

bool fd_owner::is_open() const noexcept {
    return fd_ != -1;
}

fd_owner::operator int() const noexcept {
    return fd_;
}

} // namespace ipc
