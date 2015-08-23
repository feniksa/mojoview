project(mojoview)

cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED)

find_package(InDB8 REQUIRED)
include_directories(${INDB8_INCLUDE_DIR})
set (LIBS ${LIBS} ${INDB8_LIBRARIES})

add_definitions(${INDB8_CFLAGS})
add_definitions("-std=c++11")

add_subdirectory(src)


