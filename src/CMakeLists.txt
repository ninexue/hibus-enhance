cmake_minimum_required(VERSION 3.5)
project(hibus-busybox)

FIND_LIBRARY(libhibox NAMES hibox)
FIND_LIBRARY(libhibus NAMES hibus)

# get all header files
file(COPY hibus-busybox.h DESTINATION ${BUSYBOX_PUBLIC_HEADER_DIR})

AUX_SOURCE_DIRECTORY(./ DIR_SRCS)

# make library
ADD_LIBRARY(hibus-busybox SHARED ${DIR_SRCS})
TARGET_LINK_LIBRARIES(hibus-busybox pthread dl ${libhibox} ${libhibus})

INSTALL(TARGETS hibus-busybox LIBRARY DESTINATION lib)
INSTALL(FILES hibus-busybox.h DESTINATION include)
