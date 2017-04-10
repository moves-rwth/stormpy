cmake_minimum_required(VERSION 3.0.0)
project(pycarl)


find_package(carl REQUIRED)
add_subdirectory(resources/pybind11)

set(PYCARL_USE_CLN ${CARL_USE_CLN_NUMBERS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/definitions.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/definitions.h)

file(GLOB_RECURSE PYCARL_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp)

pybind11_add_module(core ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_core.cpp ${PYCARL_CORE_SOURCES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(core PRIVATE lib_carl)


file(GLOB_RECURSE PYCARL_FORMULA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/formula/*.cpp)

pybind11_add_module(formula ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_formula.cpp ${PYCARL_FORMULA_SOURCES})
target_include_directories(formula PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(formula PRIVATE lib_carl)


file(GLOB_RECURSE PYCARL_PARSE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/*.cpp)

pybind11_add_module(parse ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_parse.cpp ${PYCARL_PARSE_SOURCES})
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(parse PRIVATE lib_carl)



