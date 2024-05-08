cmake_minimum_required(VERSION 3.25)

project(game CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(src)
