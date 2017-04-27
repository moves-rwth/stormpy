cmake_minimum_required(VERSION 3.0.0)
project(storm-version)
find_package(storm REQUIRED)

# Check for storm-dft
if(EXISTS "${storm_DIR}/lib/libstorm-dft.dylib")
    set(HAVE_STORM_DFT TRUE)
elseif(EXISTS "${storm_DIR}/lib/libstorm-dft.so")
    set(HAVE_STORM_DFT TRUE)
else()
    set(HAVE_STORM_DFT FALSE)
endif()

# Set configuration
set(STORM_DIR ${storm_DIR})

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
