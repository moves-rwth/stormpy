cmake_minimum_required(VERSION 3.0.0)
project(carl-version)
find_package(carl REQUIRED)

# Set configuration
set(CARL_DIR ${carl_DIR})
set(CARL_VERSION ${carl_VERSION})


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.py.in ${CMAKE_CURRENT_BINARY_DIR}/generated/config.py @ONLY)
