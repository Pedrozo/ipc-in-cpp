cmake_minimum_required(VERSION 3.0)

project(IPC_ON_CPP
        VERSION 0.1
        DESCRIPTION "C++ wrappers for IPC sockets")

add_library(ipc_on_cpp STATIC)

target_sources(ipc_on_cpp PRIVATE
               src/server.cpp
               src/fd_owner.cpp
               src/connection.cpp
               src/type.cpp
               src/address.cpp
               src/errno_except.cpp
               src/fd_device.cpp)

target_include_directories(ipc_on_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

find_package(Boost 1.79 REQUIRED COMPONENTS iostreams)

target_link_libraries(ipc_on_cpp PRIVATE Boost::iostreams)