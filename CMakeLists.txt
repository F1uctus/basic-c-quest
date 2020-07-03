cmake_minimum_required(VERSION 3.16)
project(rpg C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(rpg ui_tools.c ui_tools.h string_tools.c string_tools.h main.c game_map.c game_map.h)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})