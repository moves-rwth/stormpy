cmake_minimum_required(VERSION 3.0.0)
project(storm-version)
find_package(storm REQUIRED)


# Set configuration
set(STORM_DIR ${storm_DIR})
set(STORM_VERSION ${storm_VERSION})
set(STORM_LIBS ${storm_LIBRARIES})

# Check for storm-pars
find_library(STORM_PARS NAMES storm-pars HINTS "${storm_DIR}/lib/")
if(STORM_PARS)
    set(HAVE_STORM_PARS TRUE)
else()
    set(HAVE_STORM_PARS FALSE)
endif()

# Check for storm-dft
find_library(STORM_DFT NAMES storm-dft HINTS "${storm_DIR}/lib/")
if(STORM_DFT)
    set(HAVE_STORM_DFT TRUE)
else()
    set(HAVE_STORM_DFT FALSE)
endif()

if(HAVE_STORM_PARS)
    set(HAVE_STORM_PARS_BOOL "True")
else()
    set(HAVE_STORM_PARS_BOOL "False")
endif()

if(HAVE_STORM_DFT)
    set(HAVE_STORM_DFT_BOOL "True")
else()
    set(HAVE_STORM_DFT_BOOL "False")
endif()

if(STORM_USE_CLN_EA)
    set(STORM_CLN_EA_BOOL "True")
else()
    set(STORM_CLN_EA_BOOL "False")
endif()

if(STORM_USE_CLN_RF)
    set(STORM_CLN_RF_BOOL "True")
else()
    set(STORM_CLN_RF_BOOL "False")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.py.in ${CMAKE_CURRENT_BINARY_DIR}/generated/config.py @ONLY)
