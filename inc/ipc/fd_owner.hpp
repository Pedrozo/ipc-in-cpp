#ifndef IPC_FD_OWNER_HPP_
#define IPC_FD_OWNER_HPP_

namespace ipc {

class fd_owner {
public:
    explicit fd_owner(int fd);

    ~fd_owner();

    fd_owner(const fd_owner&) = delete;

    fd_owner(fd_owner&& other) noexcept;

    fd_owner& operator=(const fd_owner&) = delete;

    fd_owner& operator=(fd_owner&& other) noexcept;

    void close();

    bool is_open() const noexcept;

    operator int() const noexcept;

private:
    int fd_;
};

} // namespace ipc

#endif // IPC_FD_OWNER_HPP_