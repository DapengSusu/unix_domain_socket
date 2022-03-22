cmake_minimum_required(VERSION 3.5)

# Set the project name
project(stream)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../build/bin/${PROJECT_NAME})

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/../../src/base.cc
    ${PROJECT_SOURCE_DIR}/../../src/stream.cc
)

# Add an executable
add_executable(${PROJECT_NAME}_server server.cc ${SOURCES})
add_executable(${PROJECT_NAME}_client client.cc ${SOURCES})

target_include_directories(
    ${PROJECT_NAME}_server
    PUBLIC ${PROJECT_SOURCE_DIR}/../../include
)

target_include_directories(
    ${PROJECT_NAME}_client
    PUBLIC ${PROJECT_SOURCE_DIR}/../../include
)
