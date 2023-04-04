cmake_minimum_required (VERSION 3.6)
project(Doodle_Jump VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 11)

set(SOURCES ./src/launch.cpp)

set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SPRITE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "data/*.png")

set(MY_LIBNAME Framework${CMAKE_BUILD_TYPE})

find_library(SDL2 NAMES "SDL2")
find_library(SDL2main NAMES "SDL2main")
find_library(SDL2_image NAMES "SDL2_image")

message("SDL2 library found in ${SDL2}")

foreach(sprite ${SPRITE_FILES})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${sprite} ${CMAKE_CURRENT_BINARY_DIR}/${sprite} COPYONLY)
endforeach()

add_library(${MY_LIBNAME} SHARED ./src/launch.cpp)

set_target_properties(${MY_LIBNAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${MY_LIBNAME} PROPERTIES PUBLIC_HEADER ./inc/Framework.h)

target_include_directories(${MY_LIBNAME} PRIVATE .)

add_executable(Doodle_Jump game.cpp MyFramework.cpp MyFramework.h posXY.h)
target_link_libraries(Doodle_Jump ${MY_LIBNAME} ${SDL2} ${SDL2main} ${SDL2_image})