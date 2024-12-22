cmake_minimum_required(VERSION 3.22)
project(RPG)

set(CMAKE_CXX_STANDARD 20)

include_directories(include/all)
include_directories(src)

if (UNIX)
    link_directories(lib/unix)
    add_executable(RPG src/main.cpp include/all/sqlite/sqlite3.c include/all/tinyxml/tinyxml2.cpp include/all/SDL_FontCache/SDL_FontCache.c)
    target_link_libraries(RPG SDL2 SDL2_image SDL2_ttf SDL2_mixer libbox2d.a libtmxlite.so theoradec theora vorbis ogg)
    set_target_properties(RPG PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
elseif(WIN32)
    link_directories(lib/win)
    include_directories(include/win)
    add_executable(RPG src/main.cpp include/all/sqlite/sqlite3.c include/all/tinyxml/tinyxml2.cpp include/all/SDL_FontCache/SDL_FontCache.c)
    target_link_libraries(RPG SDL2 SDL2main SDL2_image SDL2_ttf SDL2_mixer theora vorbis ogg box2d tmxlite)
    target_compile_definitions(RPG PRIVATE SDL_MAIN_HANDLED)

    # MSVC specific settings
#    target_compile_options(RPG PRIVATE /bigobj)
#    target_compile_definitions(RPG PRIVATE _ITERATOR_DEBUG_LEVEL=0)
endif()

add_subdirectory(src)