cmake_minimum_required(VERSION 3.0.0)
project(carl-version)
find_package(carl REQUIRED)
find_package(carlparser QUIET)


# Set configuration
set(CARL_DIR ${carl_DIR})
set(CARL_VERSION ${carl_VERSION})
if(carlparser_FOUND)
    set(CARL_PARSER_DIR ${carl_parser_DIR})
    set(CARL_WITH_PARSER "True")
else()
    set(CARL_PARSER_DIR "")
    set(CARL_WITH_PARSER "False")
endif()
if(CARL_USE_CLN_NUMBERS)
    set(CARL_WITH_CLN "True")
else()
    set(CARL_WITH_CLN "False")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.py.in ${CMAKE_CURRENT_BINARY_DIR}/generated/config.py @ONLY)
