find_package(pybind11 QUIET)
if(pybind11_FOUND)
    message(STATUS "Stormpy - Using system version of pybind11.")
else()
    message(STATUS "Stormpy - Using shipped version of pybind11.")

    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
        GIT_TAG        v2.10.0
    )

    FetchContent_MakeAvailable(pybind11)
endif()

