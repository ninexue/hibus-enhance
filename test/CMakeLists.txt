cmake_minimum_required(VERSION 3.5)
project(test)

FIND_LIBRARY(libhibox NAMES hibox)
FIND_LIBRARY(libhibus NAMES hibus)

INCLUDE_DIRECTORIES(BEFORE ${BUSYBOX_PUBLIC_HEADER_DIR})
LINK_DIRECTORIES(${BUSYBOX_PUBLIC_LIB_DIR})

AUX_SOURCE_DIRECTORY(./ DIR_SRCS)

# make library
add_executable(test ${DIR_SRCS})
TARGET_LINK_LIBRARIES(test pthread ${libhibox} ${libhibus} hibus-busybox dl)
