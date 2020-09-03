cmake_minimum_required(VERSION 3.17)
project(nqsearch)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(nqsearch main.cpp crc64.c scores.cpp checksum.cpp
        Perm.cpp Perm.h ZeroNode.cpp ZeroNode.h ThreadData.cpp ThreadData.h Log.cpp Log.h)

target_link_libraries(nqsearch pthread SDL2)
