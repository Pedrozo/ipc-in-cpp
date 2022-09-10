cmake_minimum_required(VERSION 3.1)

project(ipc_in_cpp
        VERSION 0.1
        DESCRIPTION "C++ wrappers for IPC")

add_library(ipc_in_cpp INTERFACE)

target_include_directories(ipc_in_cpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
