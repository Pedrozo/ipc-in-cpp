# ipc-in-cpp
Here you will find simple C++ wrappers for inter-process communication facilities. The goal is to make IPC more simple and readable, without adding (much) performance overhead. Thus important C++ features/patterns/practices were used, such as namespaces, exceptions, and RAII!

**Note:** For those who wish to use this on Android (as do I), the abstract namespace used by Android is also covered when you cross-compile to it.

## Examples
### Sockets (UNIX domain)
UNIX domain sockets allow a bidirectional communication between processes. Such feature is available in `ipc::socket::connection`, which has functions for sending and receiving data through a client-server connection. The `ipc::socket::server` represents the server, and its `accept()` function waits clients connections. In addition, clients must use the `ipc::socket::connect()` to connect to the server.
For example, the server side could be something like this:
```cpp
#include "ipc/socket/server.hpp"

// exceptions are thrown in case of errors
void server_side() {
    // the type of the connection you'll be using
    constexpr ipc::socket::type SOCKET_TYPE = ipc::socket::type::STREAM;

    // the address that the clients will use for connecting
    constexpr const char *SOCKET_ADDRESS = "/home/foo";

    // the grow limit for pending connections
    constexpr int BACKLOG = 8;

    // creates the server that might accept connections
    ipc::socket::server my_server = ipc::socket::make_server(SOCKET_TYPE, SOCKET_ADDRESS, BACKLOG);

    // blocks until a client connects
    ipc::socket::connection con = my_server.accept();

    // and for sending data through the connection, just use its send function (analogous to write())
    float some_data = 0.666f;
    con.send((const uint_8 *) &some_data, sizeof(some_data));

    // RAII will take care of properly closing the server and the connection
}
```

And the client side that connects to that server could be:
```cpp
#include "ipc/socket/connection.hpp"

// exceptions are thrown in case of errors
void client_side() {
    // the type of the connection you'll be using
    constexpr ipc::socket::type SOCKET_TYPE = ipc::socket::type::STREAM;

    // the address that the clients will use for connecting
    constexpr const char *SOCKET_ADDRESS = "/home/foo";

    // attempt to connect to the server
    ipc::socket::connection con = ipc::socket::connect(SOCKET_TYPE, SOCKET_ADDRESS);

    // receives data from the server
    float some_data;
    con.recv((uint_8 *) &some_data, sizeof(some_data));

    // the connection will be closed per RAII
}
```
