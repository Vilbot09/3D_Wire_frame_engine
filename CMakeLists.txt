cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(engine VERSION 1.0.0 LANGUAGES C CXX)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB SOURCES "src/*.cpp")

if (WIN32) 
    message(STATUS "Creating cmake files for windows!")
    link_directories(${CMAKE_SOURCE_DIR}/libs/windows)
    add_executable(engine ${SOURCES})
    target_link_libraries(engine raylib gdi32 winmm -Wno-deprecated)


elseif (UNIX) 
    message(STATUS "Creating cmake files for linux!")
    link_directories(${CMAKE_SOURCE_DIR}/libs/linux)
    add_executable(engine ${SOURCES})
    target_link_libraries(engine raylib m dl pthread GL rt X11 -Wno-deprecated)


else() 
    message(FATAL_ERROR "Platform Not Supported")


endif()
