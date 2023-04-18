cmake_minimum_required(VERSION 3.25)
project(PhysEngine_SDL)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(engine main.c Engine_SDL.c engine.h SDL2_gfxPrimitives.c SDL2_imageFilter.c
        SDL2_rotozoom.c SDL2_framerate.c circle.h circle.c collisionPair.h)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(engine PRIVATE ${SDL2_LIBRARIES})





