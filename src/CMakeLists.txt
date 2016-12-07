# This is a CMake example for Python

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src ${pycarl_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${pybind11_incdir})

FILE(GLOB core_files "core/*.cpp")
FILE(GLOB formula_files "formula/*.cpp")
FILE(GLOB parse_files "parse/*.cpp")
ADD_LIBRARY(pycarl_core SHARED mod_core.cpp ${core_files})
ADD_LIBRARY(pycarl_formula SHARED mod_formula.cpp ${formula_files})
ADD_LIBRARY(pycarl_parse SHARED mod_parse.cpp ${parse_files})

SET(PYCARL_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
SET(PYCARL_OUTPUT_DIR "${PROJECT_BINARY_DIR}/pycarl")

SET_TARGET_PROPERTIES(pycarl_core PROPERTIES
        OUTPUT_NAME core
        PREFIX ""
        SUFFIX ".so"
        LIBRARY_OUTPUT_DIRECTORY ${PYCARL_OUTPUT_DIR}/
)

SET_TARGET_PROPERTIES(pycarl_formula PROPERTIES
        OUTPUT_NAME formula
        PREFIX ""
        SUFFIX ".so"
        LIBRARY_OUTPUT_DIRECTORY ${PYCARL_OUTPUT_DIR}/formula
)

SET_TARGET_PROPERTIES(pycarl_parse PROPERTIES
        OUTPUT_NAME parse
        PREFIX ""
        SUFFIX ".so"
        LIBRARY_OUTPUT_DIRECTORY ${PYCARL_OUTPUT_DIR}/parse
)

add_custom_target(pycarl_files)
add_custom_command(TARGET pycarl_files POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_SOURCE_DIR}/pycarl ${PYCARL_OUTPUT_DIR}
    )

if(BUILD_STATIC)
    TARGET_LINK_LIBRARIES(pycarl_core lib_carl_static  ${pycarl_LIBRARIES_STATIC} ${PYTHON_LIBRARIES} ${libs_static} )
    TARGET_LINK_LIBRARIES(pycarl_formula lib_carl_static ${pycarl_LIBRARIES_STATIC} ${PYTHON_LIBRARIES} ${libs_static} )
    TARGET_LINK_LIBRARIES(pycarl_parse lib_carl_static ${pycarl_LIBRARIES_STATIC} ${PYTHON_LIBRARIES} ${libs_static} )
else()
    TARGET_LINK_LIBRARIES(pycarl_core lib_carl ${pycarl_LIBRARIES_DYNAMIC} ${PYTHON_LIBRARIES} ${libs_dynamic})
    TARGET_LINK_LIBRARIES(pycarl_formula lib_carl ${pycarl_LIBRARIES_DYNAMIC} ${PYTHON_LIBRARIES} ${libs_dynamic})
    TARGET_LINK_LIBRARIES(pycarl_parse lib_carl ${pycarl_LIBRARIES_DYNAMIC} ${PYTHON_LIBRARIES} ${libs_dynamic})
endif()
#
add_custom_target(pycarl DEPENDS pycarl_files pycarl_core pycarl_formula pycarl_parse)

