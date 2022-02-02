cmake_minimum_required(VERSION 3.21)
project(curdle C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/find)
set(CMAKE_BUILD_TYPE Debug)


find_package(PkgConfig REQUIRED)
find_package(SDL2TTF REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(curdle main.c game.c render.c game.h render.h words.c words.h statistics.c statistics.h)

target_link_libraries(curdle ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY})
target_include_directories(curdle PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
target_compile_options(curdle PUBLIC ${SDL2_CFLAGS_OTHER})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -g -Wall -Werror")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -g -Wall -Werror")
