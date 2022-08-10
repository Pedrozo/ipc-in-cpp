#ifndef IPC_FD_DEVICE_HPP_
#define IPC_FD_DEVICE_HPP_

#include <ios>
#include <boost/iostreams/categories.hpp>

namespace ipc {

class fd_device {
public:

    using char_type = char;
    using category = boost::iostreams::bidirectional_device_tag;

    explicit fd_device(int fd);

    std::streamsize read(char_type *dest, std::streamsize size);

    std::streamsize write(const char_type* src, std::streamsize size);

private:
    int fd_;
};

} // namespace ipc


#endif // IPC_FD_DEVICE_HPP_