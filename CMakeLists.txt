cmake_minimum_required(VERSION 3.0.0)
project(pycarl)


find_package(carl REQUIRED)
add_subdirectory(resources/pybind11)

set(PYCARL_USE_CLN CARL_USE_CLN_NUMBERS)

file(GLOB_RECURSE PYCARL_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp)

pybind11_add_module(pycarl.core ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_core.cpp ${PYCARL_CORE_SOURCES})
target_include_directories(pycarl.core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pycarl.core PRIVATE lib_carl)


file(GLOB_RECURSE PYCARL_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/formula/*.cpp)

pybind11_add_module(pycarl.formula ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_formula.cpp ${PYCARL_FORMULA_SOURCES})
target_include_directories(pycarl.formula PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pycarl.formula PRIVATE lib_carl)


file(GLOB_RECURSE PYCARL_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/parse/*.cpp)

pybind11_add_module(pycarl.parse ${CMAKE_CURRENT_SOURCE_DIR}/src/mod_parse.cpp ${PYCARL_PARSE_SOURCES})
target_include_directories(pycarl.parse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pycarl.parse PRIVATE lib_carl)




if (FALSE)

	# Collect include directories
	set(PYCARL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/src)
	set(PYCARL_DEFINE_CLN "#define=PYCARL_USE_CLN")
	set(PYCARL_LIBRARIES "gmp,gmpxx")
	if(USE_CLN_NUMBERS)
		get_include_dir(PYCARL_INCLUDE_DIRS CLN_SHARED)
		get_include_dir(PYCARL_INCLUDE_DIRS CLN_STATIC)
		get_include_dir(PYCARL_INCLUDE_DIRS GMP_STATIC)
		set(PYCARL_DEFINE_CLN "define=PYCARL_USE_CLN")
		set(PYCARL_LIBRARIES "cln")
	endif()
	if(USE_GINAC)
		get_include_dir(PYCARL_INCLUDE_DIRS GINAC_SHARED)
		get_include_dir(PYCARL_INCLUDE_DIRS GINAC_STATIC)
		get_include_dir(PYCARL_INCLUDE_DIRS CLN_STATIC)
	endif()
	get_include_dir(PYCARL_INCLUDE_DIRS GMP_SHARED)
	get_include_dir(PYCARL_INCLUDE_DIRS GMP_STATIC)
	get_include_dir(PYCARL_INCLUDE_DIRS GMPXX_STATIC)
	get_include_dir(PYCARL_INCLUDE_DIRS EIGEN3)
	#get_include_dir(PYCARL_INCLUDE_DIRS GTESTCORE_STATIC)
	#get_include_dir(PYCARL_INCLUDE_DIRS GTESTMAIN_STATIC)

	# Set configuration file
	set(PYCARL_LIBRARY_DIRS "${CMAKE_BINARY_DIR}")
	set(PYCARL_RPATH "${CMAKE_BINARY_DIR}")

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg.in ${CMAKE_CURRENT_SOURCE_DIR}/setup.cfg)
	add_custom_target(PythonBindingsBuild
		COMMAND CC=${CMAKE_CXX_COMPILER} ${PYTHON_EXECUTABLE} setup.py build
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DEPENDS lib_carl
	)
	add_custom_target(PythonBindingsDevelop
		COMMAND CC=${CMAKE_CXX_COMPILER} ${PYTHON_EXECUTABLE} setup.py develop
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DEPENDS lib_carl
	)
	add_custom_target(PythonBindingsInstall
		COMMAND CC=${CMAKE_CXX_COMPILER} ${PYTHON_EXECUTABLE} setup.py install --user
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DEPENDS lib_carl
	)
endif()
