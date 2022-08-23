#ifndef IPC_SOCKET_ADDRESS_HPP_
#define IPC_SOCKET_ADDRESS_HPP_

#include <string>
#include <sys/socket.h>
#include <sys/un.h>

namespace ipc {

namespace socket {

class address {
public:
    static const std::size_t MAX_LEN;

    address(const char *str);

    address(const std::string& str);

    void swap(address& other) noexcept;

    const char *as_str() const noexcept;

    const struct sockaddr_un& as_unix() const noexcept;

private:
    struct sockaddr_un addr_;
};

void swap(address& a1, address& a2) noexcept;

} // namespace socket

} // namespace ipc


#endif // IPC_SOCKET_ADDRESS_HPP_
