cmake_minimum_required(VERSION 3.0.0)
project(pystorm)

find_package(storm REQUIRED)
add_subdirectory(resources/pybind11)

option(STORMPY_DISABLE_SIGNATURE_DOC "Disable the signature in the documentation" OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/generated/config.h)

message(STATUS "STORM-DIR: ${storm_DIR}")
message(STATUS "STORM-INCLUDE-DIR: ${storm_INCLUDE_DIR}")

function(stormpy_module NAME)
    # second, optional argument are ADDITIONAL_LIBRARIES
    # third, optional argument are ADDITIONAL_INCLUDES

    file(GLOB_RECURSE "STORM_${NAME}_SOURCES" "${CMAKE_CURRENT_SOURCE_DIR}/src/${NAME}/*.cpp")
    pybind11_add_module(${NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/mod_${NAME}.cpp" ${STORM_${NAME}_SOURCES})

    if(ARGC GREATER 1)
        # Additional libraries
        target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR} ${storm-parsers_INCLUDE_DIR} ${storm-counterexamples_INCLUDE_DIR} ${storm-version-info_INCLUDE_DIR} ${ARGV2} ${CMAKE_CURRENT_BINARY_DIR}/src/generated)
        target_link_libraries(${NAME} PRIVATE storm storm-parsers storm-counterexamples storm-version-info ${ARGV1})
    else()
        target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${storm_INCLUDE_DIR} ${storm-parsers_INCLUDE_DIR} ${storm-counterexamples_INCLUDE_DIR} ${storm-version-info_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/src/generated)
        target_link_libraries(${NAME} PRIVATE storm storm-parsers storm-counterexamples storm-version-info )
    endif()
endfunction(stormpy_module)

stormpy_module(core)
stormpy_module(info)
stormpy_module(logic)
stormpy_module(storage)
stormpy_module(utility)

# Optional modules
if(HAVE_STORM_DFT)
    stormpy_module(dft storm-dft "${storm-dft_INCLUDE_DIR}")
endif()
if(HAVE_STORM_GSPN)
    stormpy_module(gspn storm-gspn "${storm-gspn_INCLUDE_DIR}")
endif()
if(HAVE_STORM_PARS)
    stormpy_module(pars storm-pars "${storm-pars_INCLUDE_DIR}")
endif()
if(HAVE_STORM_POMDP)
    stormpy_module(pomdp storm-pomdp "${storm-pomdp_INCLUDE_DIR}")
endif()

