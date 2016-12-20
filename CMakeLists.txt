cmake_minimum_required(VERSION 3.0.0)
project(pystorm)


option(STORMPY_DISABLE_SIGNATURE_DOC "disables the signature in the documentation" ON)

find_package(storm REQUIRED)
add_subdirectory(resources/pybind11)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/generated/config.h) 


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
    target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src/generated)
    target_link_libraries(${NAME} PRIVATE storm)

    # setup.py will override this (because pip may want a different install
    # path), but also specifying it here has the advantage that invoking cmake
    # manually uses the correct path if the default is used (i.e. the
    # STORMPY_LIB_DIR hardcoded above)
    set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${LIB_PATH}")
endfunction(stormpy_module)


stormpy_module(core "${STORMPY_LIB_DIR}")
stormpy_module(info)
stormpy_module(expressions)
stormpy_module(logic)
stormpy_module(storage)
stormpy_module(utility)
