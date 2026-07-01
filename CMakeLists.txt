cmake_minimum_required(VERSION 3.20)

project(NovakExchange)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(NovakExchange ${SOURCES})

target_link_libraries(NovakExchange ws2_32)