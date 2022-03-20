cmake_minimum_required(VERSION 3.5)

# Set the project name
project(unix_domain_socket)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

# Add an executable
add_executable(server src/server.cc src/base.cc)
add_executable(client src/client.cc src/base.cc)

target_include_directories(
    server
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(
    client
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
