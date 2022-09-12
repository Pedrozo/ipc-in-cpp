#ifndef IPC_UTILS_HPP
#define IPC_UTILS_HPP

#include "SyscallError.hpp"

namespace ipc {

template<typename func, typename... args>
constexpr auto CallSys(const func& f, args&&... ar) {
    auto ret = f(std::forward<args>(ar)...);
    if (ret == -1)
        throw SyscallError(errno);
    return ret;
}

} // namespace ipc

#endif // IPC_UTILS_HPP