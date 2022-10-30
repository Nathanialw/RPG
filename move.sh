#! /bin/sh

#copy files to run debug
cmake -E copy_directory ./maps ./cmake-build-debug/maps
cmake -E copy_directory ./sprites ./cmake-build-debug/sprites
cmake -E copy_directory ./sounds ./cmake-build-debug/sounds
cmake -E copy_directory ./fonts ./cmake-build-debug/fonts
cmake -E copy_directory ./db ./cmake-build-debug/db

#copy files to run release
cmake -E copy_directory ./maps ./cmake-build-release/maps
cmake -E copy_directory ./sprites ./cmake-build-release/sprites
cmake -E copy_directory ./sounds ./cmake-build-release/sounds
cmake -E copy_directory ./fonts ./cmake-build-release/fonts
cmake -E copy_directory ./db ./cmake-build-debug/db



