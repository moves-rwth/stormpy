cmake_minimum_required(VERSION 3.14) # Needed for FetchContent_MakeAvailable

project(pycarl)

option(USE_CLN_NUMBERS "Make cln numbers available in pycarl" ON)
option(USE_PARSER "Make carlparser available in pycarl" ON)
set(PYBIND_VERSION "" CACHE STRING "Pybind11 version to use")
MARK_AS_ADVANCED(PYBIND_VERSION)

find_package(carl REQUIRED)
find_package(carlparser QUIET)

include(resources/include_pybind11.cmake)

set(CMAKE_CXX_STANDARD 14)

# This sets interprocedural optimization off as this leads to some problems on some systems
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)


# Set configurations
set(CARL_VERSION ${carl_VERSION})
if ((USE_PARSER) AND (carlparser_FOUND))
    set(CARL_WITH_PARSER "True")
else()
    set(CARL_WITH_PARSER "False")
endif()
if ((USE_CLN_NUMBERS) AND (CARL_USE_CLN_NUMBERS))
    set(CARL_WITH_CLN "True")
    set(PYCARL_HAS_CLN TRUE)
else()
    set(CARL_WITH_CLN "False")
    set(PYCARL_HAS_CLN FALSE)
endif()

# Generate definitions used during compilation
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/definitions.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/definitions.h)

# Extensions
############

# Core
######
file(GLOB_RECURSE PYCARL_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp)
pybind11_add_module(core ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_core.cpp ${PYCARL_CORE_SOURCES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(core PRIVATE lib_carl)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/core_config.py.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/_config.py @ONLY)

# Typed core
file(GLOB_RECURSE PYCARL_TYPED_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/typed_core/*.cpp)
# GMP
pybind11_add_module(gmp ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_gmp.cpp ${PYCARL_TYPED_CORE_SOURCES})
target_include_directories(gmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(gmp PRIVATE lib_carl)
set_target_properties(gmp PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/gmp" OUTPUT_NAME "gmp")
# CLN
pybind11_add_module(cln ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_cln.cpp ${PYCARL_TYPED_CORE_SOURCES})
target_include_directories(cln PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(cln PRIVATE lib_carl)
target_compile_definitions(cln PUBLIC "PYCARL_USE_CLN=ON")
set_target_properties(cln PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cln" OUTPUT_NAME "cln")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cln_config.py.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cln/_config.py @ONLY)


# Formula
#########
file(GLOB_RECURSE PYCARL_FORMULA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/formula/*.cpp)
pybind11_add_module(formula ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_formula.cpp ${PYCARL_FORMULA_SOURCES})
target_include_directories(formula PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(formula PRIVATE lib_carl)
set_target_properties(formula PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/formula" OUTPUT_NAME "formula")

# Typed formula
file(GLOB_RECURSE PYCARL_TYPED_FORMULA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/typed_formula/*.cpp)
# GMP
pybind11_add_module(formula-gmp ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_formula.cpp ${PYCARL_TYPED_FORMULA_SOURCES})
target_include_directories(formula-gmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(formula-gmp PRIVATE lib_carl)
set_target_properties(formula-gmp PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/gmp/formula" OUTPUT_NAME "formula")
# CLN
pybind11_add_module(formula-cln ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_formula.cpp ${PYCARL_TYPED_FORMULA_SOURCES})
target_include_directories(formula-cln PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(formula-cln PRIVATE lib_carl)
target_compile_definitions(formula-cln PUBLIC "PYCARL_USE_CLN=ON")
set_target_properties(formula-cln PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cln/formula" OUTPUT_NAME "formula")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cln_config.py.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cln/formula/_config.py @ONLY)


# Parse
#######
file(GLOB_RECURSE PYCARL_PARSE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/*.cpp)

pybind11_add_module(parse ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_parse.cpp ${PYCARL_PARSE_SOURCES})
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(parse PRIVATE lib_carl carl-parser)
set_target_properties(parse PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/parse" OUTPUT_NAME "parse")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/parser_config.py.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/parse/_config.py @ONLY)

# Typed parse
file(GLOB_RECURSE PYCARL_TYPED_PARSE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/typed_parse/*.cpp)
# GMP
pybind11_add_module(parse-gmp ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_parse.cpp ${PYCARL_TYPED_PARSE_SOURCES})
target_include_directories(parse-gmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(parse-gmp PRIVATE lib_carl carl-parser)
set_target_properties(parse-gmp PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/gmp/parse" OUTPUT_NAME "parse")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/parser_config.py.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/gmp/parse/_config.py @ONLY)
# CLN
pybind11_add_module(parse-cln ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_parse.cpp ${PYCARL_TYPED_PARSE_SOURCES})
target_include_directories(parse-cln PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(parse-cln PRIVATE lib_carl carl-parser)
target_compile_definitions(parse-cln PUBLIC "PYCARL_USE_CLN=ON")
set_target_properties(parse-cln PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cln/parse" OUTPUT_NAME "parse")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/parser_config.py.in ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cln/parse/_config.py @ONLY)
