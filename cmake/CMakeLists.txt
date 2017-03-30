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

message(STATUS "STORM-DIR: ${storm_DIR}")
message(STATUS "HAVE-STORM-DFT: ${HAVE_STORM_DFT}")
