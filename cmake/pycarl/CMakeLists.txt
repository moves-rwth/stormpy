cmake_minimum_required(VERSION 3.15)

project(carl-version)
set(CARL_DIR_HINT "" CACHE STRING "A hint where the Carl library can be found.")
set(CARLPARSER_DIR_HINT "" CACHE STRING "A hint where the Carl-parser library can be found.")

find_package(carl REQUIRED HINTS ${CARL_DIR_HINT})
find_package(carlparser QUIET HINTS ${CARLPARSER_DIR_HINT})


# Set configuration
set(CARL_DIR ${carl_DIR})
set(CARL_VERSION ${carl_VERSION})
if(carlparser_FOUND)
    set(CARL_PARSER_DIR ${carlparser_DIR})
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

# Write configuration file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.py.in ${CMAKE_CURRENT_BINARY_DIR}/generated/config.py @ONLY)
