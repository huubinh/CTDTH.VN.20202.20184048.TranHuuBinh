project(CTD)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_STANDARD 99)

add_subdirectory(libfdr)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/libfdr)


add_executable(main buoi1/main.c)
target_link_libraries(main fdr)