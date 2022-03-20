cmake_minimum_required(VERSION 3.5)

# Set the project name
project(datagram)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../build/bin/${PROJECT_NAME})

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/../../src/base.cc
    ${PROJECT_SOURCE_DIR}/../../src/datagram.cc
)

# Add an executable
add_executable(server server.cc ${SOURCES})
add_executable(client client.cc ${SOURCES})

target_include_directories(
    server
    PUBLIC ${PROJECT_SOURCE_DIR}/../../include
)

target_include_directories(
    client
    PUBLIC ${PROJECT_SOURCE_DIR}/../../include
)
