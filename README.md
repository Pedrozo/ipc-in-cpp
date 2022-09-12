# ipc-in-cpp
Here you will find simple C++ wrappers for inter-process communication facilities. The goal is to make IPC more simple and readable, without adding (much) performance overhead. Thus important C++ features/patterns/practices were used, such as namespaces, exceptions, and of couse RAII!
## Sockets (UNIX domain)
UNIX domain sockets allow a bidirectional communication between processes. But before using them, you have 
### Connection-less communication
A connection is not mandatory for communicating using sockets. You can instead bind a socket to an address and use it send/receive data to/from another address. In UNIX systems, such approach is available for Datagram sockets, and here it's available in the `ipc::socket::Endpoint` class. Since examples are always helpful, following there's one with an "echo" server that returns the received message as upper case.

```cpp
#include <iostream>
#include "ipc/socket/Endpoint.hpp"

#define MAX_MSG_SIZE 30
#define SERVER_FILE "/tmp/my_sock_server"

void ToUpperCase(char *str) {
    while (*str) {
        *str = static_cast<char>(toupper(*str));
        str++;
    }
}

void RunEchoServer() {
    // the address that the endpoint will bind to
    // it will be a actual file in the file system because we're using MakePathAddress()
    ipc::socket::Address addr = ipc::socket::MakePathAddress(SERVER_FILE);

    // creates a datagram socket bind to the /tmp/my_sock_server
    ipc::socket::Endpoint endpoint = ipc::socket::MakeEndpoint(addr);

    char msg[MAX_MSG_SIZE + 1];
    msg[MAX_MSG_SIZE] = '\0';

    // loop receiving strings and replying them to the sender as upper case
    for (;;) {
        ipc::socket::Address replyAddress;
        ssize_t msgLen = endpoint.Recv(msg, MAX_MSG_SIZE, replyAddress);

        ToUpperCase(msg);

        endpoint.Send(msg, msgLen, replyAddress);
    }

    // no need to close the socket or anything, RAII will take care of closing everything correctly
}

int main() {
    remove(SERVER_FILE);

    try {
        RunEchoServer();
    } catch (const ipc::SyscallError& err) {
        // the SyscallError is a wrapper for system call erros (i.e. errno codes)
        printf("[error] what: %s\n", err.what());
    }

    return 0;
}
```

And below it's a possible client for that server.

```cpp
#include <iostream>
#include "ipc/socket/Endpoint.hpp"

#define MAX_MSG_SIZE 1024
#define SERVER_FILE "/tmp/my_sock_server"
#define CLIENT_FILE "/tmp/my_sock_client"

void RunClient() {
    // the the address that this client endpoint binds to
    ipc::socket::Address myAddress = ipc::socket::MakePathAddress(CLIENT_FILE);

    // the target address of our messages
    ipc::socket::Address recipientAddress = ipc::socket::MakePathAddress(SERVER_FILE);

    // Creates a datagram socket bind to the client address
    ipc::socket::Endpoint endpoint = ipc::socket::MakeEndpoint(myAddress);

    char msg[MAX_MSG_SIZE + 1];
    msg[MAX_MSG_SIZE] = '\0';

    // loop reading strings from the stdin, sending them to the server socket, and writing
    // the server's response to the stdout
    for (;;) {
        printf("? ");
        scanf(" %s", msg);

        endpoint.Send(msg, strlen(msg) + 1, recipientAddress);
        endpoint.Recv(msg, MAX_MSG_SIZE);

        printf("> %s\n", msg);
    }

    // no need to close the socket or anything, RAII will take care of closing everything correctly
}

int main() {
    // remove the client address file if it exists
    remove(CLIENT_FILE);

    try {   
        RunClient();
    } catch (const ipc::SyscallError& err) {
        // the SyscallError is a wrapper for system call erros (i.e. errno codes)
        printf("[error] what: %s\n", err.what());
    }
    

    return 0;
}
```

### Connection based communication
If a connection is indeed required you can use the `ipc::socket::Server` and the `ipc::socket::Connection` classes. Two types of server/connection are available: `ipc::socket::Type::STREAM` and `ipc::socket::Type::PACKET`, analogous to `SOCK_STREAM` and `SOCK_SEQPACKET` from UNIX.
In the following example the server writes all the characters it receives into a file.
```cpp
#include <iostream>
#include <fstream>
#include "ipc/socket/Server.hpp"

constexpr std::size_t BUFFER_SIZE = 1024;
#define SOCKET_FILE "/tmp/my_sock"

void RunServer() {
    // creates a stream server with at most 8 connections on its backlog
    ipc::socket::Server server = ipc::socket::MakeServer(
        ipc::socket::Type::STREAM,
        ipc::socket::MakePathAddress(SOCKET_FILE),
        8);

    // awaits a client to connect
    ipc::socket::Connection con = server.Accept();

    char buffer[BUFFER_SIZE + 1];
    std::ofstream output("some_file.txt"); // where the received text will be saved

    // receives data from the connection until EOF
    ssize_t bytesRecv;
    while (bytesRecv = con.Recv(buffer, BUFFER_SIZE)) {
        buffer[bytesRecv] = '\0';
        output << buffer;
    }
}

int main() {
    remove(SOCKET_FILE);

    try {
        RunServer();
    } catch (const ipc::SyscallError& err) {
        // the SyscallError is a wrapper for system call erros (i.e. errno codes)
        printf("[error] what: %s\n", err.what());
    }

    return 0;
}
```

And the following client reads multiple lines from the `stdin` and forwards them to the server through the connection.
```cpp
#include <iostream>
#include <algorithm>
#include "ipc/socket/Connection.hpp"

constexpr std::size_t BUFFER_SIZE = 1024;
#define SOCKET_FILE "/tmp/my_sock"

void RunClient() {
    auto address = ipc::socket::MakePathAddress(SOCKET_FILE);

    // connect to a stream server
    auto con = ipc::socket::Connect(ipc::socket::Type::STREAM, address);

    // read lines from the stdin until EOF
    std::string line;
    while (std::getline(std::cin, line)) {
        const char *data = line.c_str();
        std::size_t len = line.length();

        // sends the line through the socket connection
        while (len > 0) {
            ssize_t bytesSent = con.Send(data, std::min(len, BUFFER_SIZE));
            data += bytesSent;
            len -= bytesSent;
        }

        char endline = '\n';
        con.Send(&endline, sizeof(endline));
    }
}

int main() {
    try {   
        RunClient();
    } catch (const ipc::SyscallError& err) {
        // the SyscallError is a wrapper for system call erros (i.e. errno codes)
        printf("[error] what: %s\n", err.what());
    }

    return 0;
}
```