cmake_minimum_required(VERSION 3.0.0)
project(pycarl)


find_package(carl REQUIRED)
find_package(carlparser QUIET)
add_subdirectory(resources/pybind11)

set(PYCARL_HAS_CLN ${CARL_USE_CLN_NUMBERS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/definitions.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/definitions.h)


# Core
file(GLOB_RECURSE PYCARL_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp)
pybind11_add_module(core ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_core.cpp ${PYCARL_CORE_SOURCES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(core PRIVATE lib_carl)

# Typed core
file(GLOB_RECURSE PYCARL_TYPED_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/typed_core/*.cpp)

pybind11_add_module(gmp ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_gmp.cpp ${PYCARL_TYPED_CORE_SOURCES})
target_include_directories(gmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(gmp PRIVATE lib_carl)

pybind11_add_module(cln ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_cln.cpp ${PYCARL_TYPED_CORE_SOURCES})
target_include_directories(cln PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(cln PRIVATE lib_carl)
target_compile_definitions(cln PUBLIC "PYCARL_USE_CLN=ON")


# Formula
file(GLOB_RECURSE PYCARL_FORMULA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/formula/*.cpp)
pybind11_add_module(formula ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_formula.cpp ${PYCARL_FORMULA_SOURCES})
target_include_directories(formula PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(formula PRIVATE lib_carl)
set_target_properties(formula PROPERTIES OUTPUT_NAME "formula")

# Typed formula
file(GLOB_RECURSE PYCARL_TYPED_FORMULA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/typed_formula/*.cpp)

pybind11_add_module(formula-gmp ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_formula.cpp ${PYCARL_TYPED_FORMULA_SOURCES})
target_include_directories(formula-gmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(formula-gmp PRIVATE lib_carl)
set_target_properties(formula-gmp PROPERTIES OUTPUT_NAME "formula")

pybind11_add_module(formula-cln ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_formula.cpp ${PYCARL_TYPED_FORMULA_SOURCES})
target_include_directories(formula-cln PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(formula-cln PRIVATE lib_carl)
target_compile_definitions(formula-cln PUBLIC "PYCARL_USE_CLN=ON")
set_target_properties(formula-cln PROPERTIES OUTPUT_NAME "formula")


# Parse
file(GLOB_RECURSE PYCARL_PARSE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/*.cpp)

pybind11_add_module(parse ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_parse.cpp ${PYCARL_PARSE_SOURCES})
target_include_directories(parse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(parse PRIVATE lib_carl carl-parser)
set_target_properties(parse PROPERTIES OUTPUT_NAME "parse")

# Typed parse
file(GLOB_RECURSE PYCARL_TYPED_PARSE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/typed_parse/*.cpp)

pybind11_add_module(parse-gmp ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_parse.cpp ${PYCARL_TYPED_PARSE_SOURCES})
target_include_directories(parse-gmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(parse-gmp PRIVATE lib_carl carl-parser)
set_target_properties(parse-gmp PROPERTIES OUTPUT_NAME "parse")

pybind11_add_module(parse-cln ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_typed_parse.cpp ${PYCARL_TYPED_PARSE_SOURCES})
target_include_directories(parse-cln PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src)
target_link_libraries(parse-cln PRIVATE lib_carl carl-parser)
target_compile_definitions(parse-cln PUBLIC "PYCARL_USE_CLN=ON")
set_target_properties(parse-cln PROPERTIES OUTPUT_NAME "parse")
