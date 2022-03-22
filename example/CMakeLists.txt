cmake_minimum_required(VERSION 3.5)

# Set the project name
project(example)

# Set bin name
set(BIN_DATAGRAM datagram)
set(BIN_STREAM stream)

# Set output dir
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

# Add an executable
add_executable(${BIN_DATAGRAM}_server datagram/server.cc)
add_executable(${BIN_DATAGRAM}_client datagram/client.cc)
add_executable(${BIN_STREAM}_server stream/server.cc)
add_executable(${BIN_STREAM}_client stream/client.cc)

# link lib
target_link_libraries(${BIN_DATAGRAM}_server lib::unix_domain_socket)
target_link_libraries(${BIN_DATAGRAM}_client lib::unix_domain_socket)
target_link_libraries(${BIN_STREAM}_server lib::unix_domain_socket)
target_link_libraries(${BIN_STREAM}_client lib::unix_domain_socket)
