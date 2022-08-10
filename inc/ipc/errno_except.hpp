#ifndef IPC_ERROR_HPP_
#define IPC_ERROR_HPP_

#include <stdexcept>

namespace ipc {

class errno_except : public std::exception {
public:

    explicit errno_except(int error_number);

    int error_number() const noexcept;

    const char *what() const noexcept override;

private:
    int error_number_;
};

} // namespace ipc

#endif // IPC_ERROR_HPP_