cmake_minimum_required(VERSION 3.21)
project(curdle C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(curdle main.c game.c game.h)

target_link_libraries(curdle ${SDL2_LIBRARIES})
target_include_directories(curdle PUBLIC ${SDL2_INCLUDE_DIRS})
target_compile_options(curdle PUBLIC ${SDL2_CFLAGS_OTHER})