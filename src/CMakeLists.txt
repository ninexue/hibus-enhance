CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(hibus-busybox)

FIND_LIBRARY(libhibox NAMES hibox)
FIND_LIBRARY(libhibus NAMES hibus)

# get all header files
FILE(COPY hibus_busybox.h DESTINATION ${BUSYBOX_PUBLIC_HEADER_DIR})

#AUX_SOURCE_DIRECTORY(./ DIR_SRCS)
set(LIB_SRCS
    interface.c
    filesystem.c
)

ADD_LIBRARY(hibus-busybox SHARED ${LIB_SRCS})
TARGET_LINK_LIBRARIES(hibus-busybox pthread dl ${libhibox} ${libhibus})
INSTALL(TARGETS hibus-busybox LIBRARY DESTINATION lib)
INSTALL(FILES hibus-busybox.h DESTINATION include)

ADD_EXECUTABLE(busybox-test ${LIB_SRCS} main.c)
TARGET_LINK_LIBRARIES(busybox-test pthread dl ${libhibox} ${libhibus})
INSTALL(TARGETS busybox-test RUNTIME DESTINATION bin)


