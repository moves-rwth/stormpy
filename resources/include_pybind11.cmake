find_package(pybind11 ${PYBIND_VERSION} CONFIG QUIET)
if(pybind11_FOUND)
    message(STATUS "Stormpy - Using system version of pybind11 (version ${pybind_VERSION}).")
else()
    message(STATUS "Stormpy - Using shipped version of pybind11 (version ${PYBIND_VERSION}).")

    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
        GIT_TAG        "v${PYBIND_VERSION}"
    )

    FetchContent_MakeAvailable(pybind11)
endif()

