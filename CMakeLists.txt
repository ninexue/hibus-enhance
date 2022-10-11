cmake_minimum_required(VERSION 3.5)
PROJECT(summer LANGUAGES C CXX)
SET(PROJECT_VERSION "1.0.0")

# set CMAKE_BUILD_TYPE = DEBUG when development, only type "make" to get debug version
IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "DEBUG" CACHE STRING "Choose the type of build." FORCE)
ELSE ()
    MESSAGE(STATUS "The CMake build type is: ${CMAKE_BUILD_TYPE}")
ENDIF ()
MESSAGE(STATUS "The CMake build type is: ${CMAKE_BUILD_TYPE}")

# set output dir, include library, include and test case
FILE(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/public/lib)
FILE(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/public/include)
FILE(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/public/test)
SET(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/public")
SET(BUSYBOX_PUBLIC_LIB_DIR "${OUTPUT_DIR}/lib")
SET(BUSYBOX_PUBLIC_HEADER_DIR "${OUTPUT_DIR}/include")

# include other options
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# set output path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/public/test)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/public/lib)

# set compile option
ADD_DEFINITIONS(-Wall -Wsign-compare -Wtype-limits -Wuninitialized -Wshift-negative-value -Wclobbered -Wignored-qualifiers -Werror -fmax-errors=10 -Wno-error=deprecated-declarations -Wno-deprecated-declarations)
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# set sub-directory
add_subdirectory(src)
add_subdirectory(test)
