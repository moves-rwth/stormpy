# Set variable to strings "True"/"False" according to condition
function(set_variable_string NAME CONDITION)
    if(CONDITION)
        set(${NAME} "True" PARENT_SCOPE)
    else()
        set(${NAME} "False" PARENT_SCOPE)
    endif()
endfunction(set_variable_string)

# Check whether Storm library is available
# Sets variable HAVE_STORM_XYZ
function(storm_with_lib NAME)
    string(TOLOWER ${NAME} NAME_LOWER)
    find_library(STORM_${NAME} NAMES storm-${NAME_LOWER} HINTS "${storm_DIR}/lib/")
    if(STORM_${NAME})
        set(HAVE_STORM_${NAME} TRUE PARENT_SCOPE)
    else()
        set(HAVE_STORM_${NAME} FALSE PARENT_SCOPE)
    endif()
endfunction(storm_with_lib)

