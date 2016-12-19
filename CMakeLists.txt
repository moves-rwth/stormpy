cmake_minimum_required(VERSION 3.0.0)
project(pystorm)


find_package(storm REQUIRED)
add_subdirectory(resources/pybind11)


set(STORMPY_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/stormpy")

function(stormpy_module NAME)
    # second, optional argument is LIBRARY_OUTPUT_DIRECTORY,
    # defaults to subdir with module name
    if(ARGC GREATER 1)
        set(LIB_PATH "${ARGV1}")
    else()
        set(LIB_PATH "${STORMPY_LIB_DIR}/${NAME}")
    endif()

    file(GLOB_RECURSE "STORM_${NAME}_SOURCES" "${CMAKE_CURRENT_SOURCE_DIR}/src/${NAME}/*.cpp")
    pybind11_add_module(${NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/mod_${NAME}.cpp" ${STORM_${NAME}_SOURCES})
    target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR})
    target_link_libraries(${NAME} PRIVATE storm)
endfunction(stormpy_module)


stormpy_module(core "${STORMPY_LIB_DIR}")
stormpy_module(info)
stormpy_module(expressions)
stormpy_module(logic)
stormpy_module(storage)
stormpy_module(utility)
