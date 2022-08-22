cmake_minimum_required(VERSION 3.0)

project(ipc_in_cpp
        VERSION 0.1
        DESCRIPTION "C++ wrappers for IPC")

add_library(ipc_in_cpp STATIC)

target_sources(ipc_in_cpp PRIVATE
               src/server.cpp
               src/fd_owner.cpp
               src/connection.cpp
               src/type.cpp
               src/address.cpp
               src/errno_except.cpp)

target_include_directories(ipc_in_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
