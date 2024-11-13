cmake_minimum_required(VERSION 3.22)
project(RPG)

set(CMAKE_VERBOSE_MAKEFILE ON)

if(UNIX)
    link_directories(lib)
elseif (WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/libwin)
endif()

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
if(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/includewin)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(RPG ${CMAKE_SOURCE_DIR}/main.cpp ${CMAKE_SOURCE_DIR}/include/sqlite/sqlite3.c ${CMAKE_SOURCE_DIR}/include/tinyxml/tinyxml2.cpp ${CMAKE_SOURCE_DIR}/include/SDL_FontCache/SDL_FontCache.c)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if (UNIX)
    target_link_libraries(RPG -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer libbox2d.a libtmxlite.so -ltheoradec -ltheora -lvorbis -logg)
    set_target_properties(RPG PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
elseif(WIN32)
    target_link_libraries(RPG SDL2 SDL2_image SDL2_ttf SDL2_mixer theora vorbis ogg box2d tmxlite)
    target_compile_definitions(RPG PRIVATE SDL_MAIN_HANDLED)
    target_compile_options(RPG PRIVATE /bigobj)
    target_compile_definitions(RPG PRIVATE _ITERATOR_DEBUG_LEVEL=0)

    #if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    #else()
    #    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    #endif()
endif()

#set_property(TARGET YourExecutable PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Set the working directory for the executable
#set_target_properties(RPG PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})