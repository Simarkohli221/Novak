cmake_minimum_required(VERSION 3.20)

project(NovakExchange)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# -------------------------
# Server Sources
# -------------------------

file(GLOB_RECURSE SERVER_SOURCES
    src/*.cpp
)

# Remove TestClient from server build
list(REMOVE_ITEM SERVER_SOURCES
    ${CMAKE_SOURCE_DIR}/src/client/TestClient.cpp
)

add_executable(
    NovakExchange
    ${SERVER_SOURCES}
)

target_link_libraries(
    NovakExchange
    ws2_32
)

# -------------------------
# Test Client
# -------------------------

add_executable(
    TestClient
    src/client/TestClient.cpp
)

target_link_libraries(
    TestClient
    ws2_32
)