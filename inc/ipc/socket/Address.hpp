#ifndef IPC_SOCKET_ADDRESS_HPP
#define IPC_SOCKET_ADDRESS_HPP

#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace ipc {

namespace socket {

class Address {
public:

    Address() : addr_(), len_(sizeof(sockaddr_un)) {}

    const sockaddr_un& UnixAddress() const {
        return addr_;
    }

    sockaddr_un& UnixAddress() {
        return addr_;
    }

    const socklen_t& Length() const {
        return len_;
    }

    socklen_t& Length() {
        return len_;
    }

private:
    sockaddr_un addr_;
    socklen_t len_;
};

inline Address MakePathAddress(const char *path) {
    Address addr;
    auto& unix_address = addr.UnixAddress();

    std::memset(&unix_address, 0, addr.Length());
    unix_address.sun_family = AF_UNIX;
    strcpy(unix_address.sun_path, path); // TODO: check bounds

    return addr;
}

inline Address MakePathAddress(const std::string& str) {
    return MakePathAddress(str.c_str());
}

inline Address MakeAbstractAddress(const char *name) {
    Address addr;
    auto& unix_address = addr.UnixAddress();

    std::memset(&unix_address, 0, addr.Length());
    unix_address.sun_family = AF_UNIX;
    strcpy(unix_address.sun_path + 1, name); // TODO: check bounds

    return addr;
}

inline Address MakeAbstractAddress(const std::string& str) {
    return MakePathAddress(str.c_str());
}

} // namespace socket

} // namespace ipc

#endif // IPC_SOCKET_ADDRESS_HPP