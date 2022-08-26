#ifndef IPC_SOCKET_ADDRESS_HPP_
#define IPC_SOCKET_ADDRESS_HPP_

#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>

namespace ipc {

namespace socket {

class address {
public:
    static const std::size_t MAX_LEN;

    void swap(address& other) noexcept;

    const char *as_str() const noexcept;

    const struct sockaddr_un& as_unix() const noexcept;

    bool is_file_path() const noexcept;

    bool is_abstract() const noexcept;

    friend address make_path_address(const char *file_path);

    friend address make_abstract_address(const char *name);

private:
    address() = default;

    struct sockaddr_un addr_;
};

void swap(address& a1, address& a2) noexcept;

address make_path_address(const char *file_path);

inline address make_path_address(const std::string& file_path) {
    return make_path_address(file_path.c_str());
}

address make_abstract_address(const char *name);

inline address make_abstract_address(const std::string& name) {
    return make_abstract_address(name.c_str());
}

} // namespace socket

} // namespace ipc


#endif // IPC_SOCKET_ADDRESS_HPP_
