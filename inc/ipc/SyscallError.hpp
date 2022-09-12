#ifndef IPC_SYSCALL_ERROR_HPP
#define IPC_SYSCALL_ERROR_HPP

#include <sys/un.h>
#include <stdexcept>

namespace ipc {

class SyscallError : public std::exception {
public:

    explicit SyscallError(int error_number) : error_number_(error_number) {}

    int ErrorNumber() const noexcept {
        return error_number_;
    }

    const char *what() const noexcept override {
        return strerror(error_number_);
    }

private:
    int error_number_;
};

} // namespace ipc

#endif // IPC_SYSCALL_ERROR_HPP