cmake_minimum_required(VERSION 3.5)
project(SO_projekt_prom C)

set(CMAKE_C_STANDARD 99)

# Put runtime executables in the source directory so `./master` can exec `./ferry` and `./passenger`.
set(OUT_DIR ${CMAKE_SOURCE_DIR}/out)
file(MAKE_DIRECTORY ${OUT_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR})

add_library(utils STATIC components/utils.c)

add_executable(master master.c)
target_link_libraries(master PRIVATE utils)

add_executable(manager components/manager.c)
target_link_libraries(manager PRIVATE utils)

add_executable(ferry components/ferry.c)
target_link_libraries(ferry PRIVATE utils)

add_executable(passenger components/passenger.c)
target_link_libraries(passenger PRIVATE utils)
