cmake_minimum_required(VERSION 3.0)
project(RPG_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(RPG_game main.cpp sqlite3.c tinyxml2.cpp)

target_link_libraries(RPG_game -lSDL2 -lSDL2_image -lSDL2_ttf /home/nathanial/Workspace/RPG_game/lib/libbox2d.a /home/nathanial/Workspace/RPG_game/lib/libtmxlite.so)
include_directories(include)