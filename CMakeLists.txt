cmake_minimum_required(VERSION 4.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(3D_Engine VERSION 1.0.0 LANGUAGES C CXX)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)
link_directories(${CMAKE_SOURCE_DIR}/libs)

file(GLOB SOURCES "src/*.cpp")

add_executable(3D_Engine ${SOURCES})

target_link_libraries(3D_Engine raylib gdi32 winmm -Wno-deprecated)