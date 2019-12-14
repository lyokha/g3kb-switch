CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

SET(name g3kb-switch)
PROJECT(${name})

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_SEARCH_MODULE(GIO REQUIRED gio-2.0)

ADD_EXECUTABLE(${name} main.c switch.c)
INCLUDE_DIRECTORIES(${GIO_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${name} ${GIO_LDFLAGS})

OPTION(BUILD_G3KBSWITCH_LIB
    "Build a library compatible with vim's libcall interface" ON)
if(BUILD_G3KBSWITCH_LIB)
    SET(g3kblib g3kbswitch)
    ADD_LIBRARY(${g3kblib} SHARED switch.c switch-api.c)
    TARGET_LINK_LIBRARIES(${g3kblib} ${GIO_LDFLAGS})
endif()

INSTALL(TARGETS ${name} ${g3kblib}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib OPTIONAL
)

