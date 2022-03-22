cmake_minimum_required(VERSION 3.5)

# Set the project name
project(unix_domain_socket)

# Set library output dir
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/base.cc
    ${PROJECT_SOURCE_DIR}/src/datagram.cc
    ${PROJECT_SOURCE_DIR}/src/stream.cc
)

# Add a library with the above sources
# add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_library(lib::unix_domain_socket ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Add sub directories
add_subdirectory(example)
