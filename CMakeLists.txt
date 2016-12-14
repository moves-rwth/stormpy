cmake_minimum_required(VERSION 3.0.0)
project(pystorm)


find_package(storm REQUIRED)
add_subdirectory(resources/pybind11)


file(GLOB_RECURSE STORM_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp)

pybind11_add_module(core ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_core.cpp ${STORM_CORE_SOURCES})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR})
target_link_libraries(core PRIVATE storm)

file(GLOB_RECURSE STORM_INFO_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/info/*.cpp)

pybind11_add_module(info ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_info.cpp ${STORM_INFO_SOURCES})
target_include_directories(info PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR})
target_link_libraries(info PRIVATE storm)


file(GLOB_RECURSE STORM_EXPRESSIONS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/expressions/*.cpp)

pybind11_add_module(expressions ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_expressions.cpp ${STORM_EXPRESSIONS_SOURCES})
target_include_directories(expressions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR})
target_link_libraries(expressions PRIVATE storm)


file(GLOB_RECURSE STORM_LOGIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/logic/*.cpp)

pybind11_add_module(logic ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_logic.cpp ${STORM_LOGIC_SOURCES})
target_include_directories(logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR})
target_link_libraries(logic PRIVATE storm)

file(GLOB_RECURSE STORM_STORAGE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/storage/*.cpp)

pybind11_add_module(storage ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_storage.cpp ${STORM_STORAGE_SOURCES})
target_include_directories(storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR})
target_link_libraries(storage PRIVATE storm)

file(GLOB_RECURSE STORM_UTILITY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/utility/*.cpp)

pybind11_add_module(utility ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_utility.cpp ${STORM_UTILITY_SOURCES})
target_include_directories(utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR})
target_link_libraries(utility PRIVATE storm)

