#ifndef IPC_FILE_DESCRIPTOR_HPP
#define IPC_FILE_DESCRIPTOR_HPP

#include <utility>
#include <unistd.h>

namespace ipc {

class FileDescriptor {
public:

    explicit FileDescriptor(int fd) : fd_(fd) {}

    ~FileDescriptor() {
        if (fd_ != -1)
            ::close(fd_);
    }

    FileDescriptor(const FileDescriptor&) = delete;

    FileDescriptor(FileDescriptor&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1;
    }

    FileDescriptor& operator=(const FileDescriptor&) = delete;

    FileDescriptor& operator=(FileDescriptor&& other) noexcept {
        std::swap(fd_, other.fd_);
        return *this;
    }

    operator int() const noexcept {
        return fd_;
    }

private:
    int fd_;
};

} // namespace ipc

#endif // IPC_FILE_DESCRIPTOR_HPP