#ifndef IPC_SOCKET_ADDRESS_HPP_
#define IPC_SOCKET_ADDRESS_HPP_

#include <string>
#include <sys/socket.h>
#include <sys/un.h>

namespace ipc {

namespace socket {

class address {
public:
    constexpr static std::size_t MAX_LEN = sizeof ((struct sockaddr_un*)0)->sun_path / sizeof(char) - 1;

    address(const char *str);

    address(const std::string& str);

    const char *str() const noexcept;

private:
    char str_[MAX_LEN + 1];
};

} // namespace socket

} // namespace ipc


#endif // IPC_SOCKET_ADDRESS_HPP_
