cmake_minimum_required(VERSION 3.5.1)
project(BWA177)

set(BWA177_VERSION_MAJOR 1)
set(BWA177_VERSION_MINOR 0)
add_definitions(-std=c++11)

include_directories(include)

aux_source_directory(src DIR_SRCS)
aux_source_directory(src/align DIR_ALIGN)
aux_source_directory(src/compressfa DIR_COMPRESSFA)
aux_source_directory(src/genBWT DIR_GENBWT)
aux_source_directory(src/genPsi DIR_GENPSI)
aux_source_directory(src/utils DIR_UTILS)

add_executable(BWA177 ${DIR_SRCS} ${DIR_ALIGN} ${DIR_COMPRESSFA} ${DIR_GENBWT} ${DIR_GENPSI} ${DIR_UTILS})
target_link_libraries(BWA177 hsds-bitvector)
target_link_libraries(BWA177 divsufsort)
