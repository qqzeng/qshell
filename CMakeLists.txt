cmake_minimum_required(VERSION 3.8)
project(qshell)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES qshell.c)
add_executable(qshell ${SOURCE_FILES})