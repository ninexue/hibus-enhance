cmake_minimum_required(VERSION 3.10)
project(inetd)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# sub-directory

set(PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
link_directories("${PROJECT_SOURCE_DIR}/lib")

# copy the necessary resource files to /bin/res
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# set output path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# set sub-directory
add_subdirectory(src)
