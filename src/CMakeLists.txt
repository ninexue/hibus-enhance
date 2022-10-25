CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(hibus-busybox)

FIND_LIBRARY(libhibox NAMES hibox)
FIND_LIBRARY(libhibus NAMES hibus)

# get all header files
#FILE(COPY hibus_busybox.h DESTINATION ${BUSYBOX_PUBLIC_HEADER_DIR})
add_custom_target(copy_header_file
    ALL
    COMMAND cp hibus_busybox.h ${BUSYBOX_PUBLIC_HEADER_DIR}
    COMMENT "copy header file for application."
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

set(LIB_SRCS
	helper.c
    interface.c
    filesystem.c
    system.c
)

ADD_LIBRARY(hibus-busybox SHARED ${LIB_SRCS})
ADD_DEPENDENCIES(hibus-busybox copy_header_file)
TARGET_LINK_LIBRARIES(hibus-busybox pthread dl ${libhibox} ${libhibus})
INSTALL(TARGETS hibus-busybox LIBRARY DESTINATION lib)
INSTALL(FILES hibus-busybox.h DESTINATION include)

ADD_EXECUTABLE(busybox-test ${LIB_SRCS} main.c)
TARGET_LINK_LIBRARIES(busybox-test pthread dl ${libhibox} ${libhibus})
INSTALL(TARGETS busybox-test RUNTIME DESTINATION bin)
