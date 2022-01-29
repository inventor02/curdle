cmake_minimum_required(VERSION 3.21)
project(curdle C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/find)

find_package(PkgConfig REQUIRED)
find_package(SDL2TTF REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
#pkg_check_modules(SDL2_ttf REQUIRED SDL2_ttf)

add_executable(curdle main.c game.c render.c game.h render.h words.c words.h)

target_link_libraries(curdle ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})
target_include_directories(curdle PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
target_compile_options(curdle PUBLIC ${SDL2_CFLAGS_OTHER})