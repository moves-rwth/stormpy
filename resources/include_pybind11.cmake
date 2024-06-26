# Always use shipped version of pybind11
message(STATUS "Pycarl - Using shipped version of pybind11 (version ${PYBIND_VERSION}).")

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG        "v${PYBIND_VERSION}"
)

FetchContent_MakeAvailable(pybind11)
