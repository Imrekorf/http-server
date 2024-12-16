cmake_minimum_required(VERSION 3.27)

project(http_server
	VERSION 0.0
	DESCRIPTION "${http_server}"
	LANGUAGES C CXX)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_STANDARD 17 CACHE STRING "Version of the C standard to use")
set(CMAKE_CXX_STANDARD 23 CACHE STRING "Version of the C++ standard to use")

option(CMAKE_C_STANDARD_REQUIRED "Enable to enforce strict C standard selection" ON)
option(CMAKE_CXX_STANDARD_REQUIRED "Enable to enforce strict C++ standard selection" ON)
option(CMAKE_C_EXTENSIONS "Enable to allow compiler-specific C extensions" OFF)

# set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to enable clangd

# cmake_presets supplies argument flags with "" making the compiler
# interpret it as a single flag, this is undesired, thus seperate the flags
separate_arguments(DEBUG_FLAGS UNIX_COMMAND ${DEBUG_FLAGS})
separate_arguments(RELEASE_FLAGS UNIX_COMMAND ${RELEASE_FLAGS})
separate_arguments(DEFINES UNIX_COMMAND ${DEFINES})
set(LINKER_FLAGS)

# find_package(<lib> <version> REQUIRED)
add_subdirectory(src/)

# note: tests requires a lib output
# add_subdirectory(lib/)
# add_subdirectory(tests/)
set(CMAKE_VERBOSE_MAKEFILE ON)