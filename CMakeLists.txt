# the name of the target operating system
#set(CMAKE_SYSTEM_NAME Windows)

cmake_minimum_required(VERSION 3.22)
project(RPG)


link_directories(lib)

set(CMAKE_CXX_STANDARD 20)
set(ARCH amd64)
#set (CMAKE_CXX_COMPILER "/usr/bin/clang++")
set (CMAKE_CXX_FLAGS "-Wno-deprecated")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std = c++11")

include_directories(include)
include_directories(src)

#add_library(srcDir "")
#add_subdirectory(src)


add_executable(RPG main.cpp sqlite3.c tinyxml2.cpp SDL_FontCache.c)
#target_sources(font SDL_FontCache.h)
add_subdirectory(src)

#for the glang compiler to see c++ std lib       -stdlib=libc++ -lc++
target_link_libraries(RPG -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer libbox2d.a libtmxlite.so -ltheoradec -ltheora -lvorbis -logg)

set_target_properties(RPG PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")



#
## adjust the default behavior of the FIND_XXX() commands:
## search programs in the host environment
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#
## search headers and libraries in the target environment
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
