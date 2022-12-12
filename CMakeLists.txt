# the name of the target operating system
#set(CMAKE_SYSTEM_NAME Windows)


cmake_minimum_required(VERSION 3.0)
project(RPG_game)


link_directories(lib)

set(CMAKE_CXX_STANDARD 20)
set(ARCH amd64)

add_executable(RPG_game main.cpp sqlite3.c tinyxml2.cpp SDL_FontCache.c)

target_link_libraries(RPG_game -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer libbox2d.a libtmxlite.so)
include_directories(include)

set_target_properties(RPG_game PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")

#
## adjust the default behavior of the FIND_XXX() commands:
## search programs in the host environment
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#
## search headers and libraries in the target environment
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)