cmake_minimum_required(VERSION 3.5.1)
project(BWA177)
add_definitions(-std=c++11)

set(BWA177_VERSION_MAJOR 1)
set(BWA177_VERSION_MINOR 0)
include_directories(include)
aux_source_directory(src DIR_SRCS)

ADD_EXECUTABLE(BWA177 ${DIR_SRCS})

target_link_libraries(BWA177 hsds-bitvector)
target_link_libraries(BWA177 divsufsort)
