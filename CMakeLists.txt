set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 3.10)

project(PCGST)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(PCGST ${SOURCES})
