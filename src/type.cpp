#include "ipc/socket/type.hpp"
#include <sys/socket.h>

namespace ipc {

namespace socket {

int to_sock(type tp) {
    switch (tp) {
        case type::STREAM:
            return SOCK_STREAM;

        case type::DGRAM:
            return SOCK_DGRAM;

        case type::SEQPACKET:
            return SOCK_SEQPACKET;

        case type::RAW:
            return SOCK_RAW;

        case type::RDM:
            return SOCK_RDM;

        case type::PACKET:
            return SOCK_PACKET;

        default:
            return -1; // never supposed to reach here
    }
}

} // namespace socket

} // namespace ipc