#include "ipc/fd_device.hpp"
#include "ipc/errno_except.hpp"

#include <unistd.h>

namespace ipc {

fd_device::fd_device(int fd) : fd_(fd) {}

std::streamsize fd_device::read(char_type *dest, std::streamsize size) {
    int ret = ::read(fd_, dest, size);

    if (ret == -1)
        throw errno_except(errno);

    return ret;
}

std::streamsize fd_device::write(const char_type *dest, std::streamsize size) {
    int ret = ::write(fd_, dest, size);

    if (ret == -1)
        throw errno_except(errno);

    return ret;
}

} // namespace ipc
