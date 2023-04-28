CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

SET(name g3kb-switch)
PROJECT(${name}
        LANGUAGES C
        HOMEPAGE_URL https://github.com/lyokha/g3kb-switch
        DESCRIPTION "CLI keyboard layout switcher for Gnome 3 and 4x"
        VERSION 1.2)

INCLUDE(GNUInstallDirs)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_SEARCH_MODULE(GIO REQUIRED gio-2.0)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

OPTION(G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION "Use Gnome Shell extension" ON)
IF(G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION)
    ADD_DEFINITIONS(-DG3KBSWITCH_WITH_GNOME_SHELL_EXTENSION)
ENDIF()

ADD_EXECUTABLE(${name} main.c switch.c)
INCLUDE_DIRECTORIES(${GIO_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${name} ${GIO_LDFLAGS})
#TARGET_COMPILE_OPTIONS(${name} PUBLIC -Wall)

OPTION(G3KBSWITCH_BUILD_VIM_XKBSWITCH_LIB "Build plugin for vim-xkbswitch" ON)
IF(G3KBSWITCH_BUILD_VIM_XKBSWITCH_LIB)
    SET(g3kblib g3kbswitch)
    ADD_LIBRARY(${g3kblib} SHARED switch.c switch-api.c)
    TARGET_LINK_LIBRARIES(${g3kblib} ${GIO_LDFLAGS})
    #TARGET_COMPILE_OPTIONS(${g3kblib} PUBLIC -Wall)
ENDIF()

OPTION(G3KBSWITCH_INSTALL_BASH_COMPLETION "Install bash completion script" ON)
IF(G3KBSWITCH_INSTALL_BASH_COMPLETION)
    FIND_PACKAGE(bash-completion)
    IF(BASH_COMPLETION_FOUND)
        INSTALL(FILES g3kb-switch-completion.bash
                DESTINATION ${BASH_COMPLETION_COMPLETIONSDIR}
                RENAME ${name})
    ENDIF()
ENDIF()

OPTION(G3KBSWITCH_INSTALL_ZSH_COMPLETION "Install zsh completion script" ON)
IF(G3KBSWITCH_INSTALL_ZSH_COMPLETION)
    SET(ZSH_COMPLETION_COMPLETIONSDIR "/usr/share/zsh/site-functions")
    INSTALL(FILES g3kb-switch-completion.zsh
            DESTINATION ${ZSH_COMPLETION_COMPLETIONSDIR}
            RENAME _${name})
ENDIF()

IF(NOT DEFINED G3KBSWITCH_VIM_XKBSWITCH_LIB_PATH)
    SET(G3KBSWITCH_VIM_XKBSWITCH_LIB_PATH ${CMAKE_INSTALL_LIBDIR})
ENDIF()
INSTALL(TARGETS ${name} ${g3kblib}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${G3KBSWITCH_VIM_XKBSWITCH_LIB_PATH} OPTIONAL)

OPTION(G3KBSWITCH_INSTALL_PC_FILE "Install pkg-config file" ON)
IF(G3KBSWITCH_INSTALL_PC_FILE)
    SET(G3KBSWITCH_PC_LIBPATH_DECL "libpath=")
    IF(G3KBSWITCH_BUILD_VIM_XKBSWITCH_LIB)
        STRING(APPEND
               G3KBSWITCH_PC_LIBPATH_DECL
               "\${prefix}/"
               ${G3KBSWITCH_VIM_XKBSWITCH_LIB_PATH}
               "/$<TARGET_FILE_NAME:g3kbswitch>")
    ENDIF()
    SET(G3KBSWITCH_PC_EXTNAME_DECL "extname=")
    IF(G3KBSWITCH_WITH_GNOME_SHELL_EXTENSION)
        STRING(APPEND
               G3KBSWITCH_PC_EXTNAME_DECL
               "g3kb-switch@g3kb-switch.org")
    ENDIF()
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/${name}.pc.in
                   ${CMAKE_BINARY_DIR}/${name}.pc.1.in
                   @ONLY)
    FILE(GENERATE
         OUTPUT ${CMAKE_BINARY_DIR}/${name}.pc
         INPUT ${CMAKE_BINARY_DIR}/${name}.pc.1.in)
    INSTALL(FILES ${CMAKE_BINARY_DIR}/${name}.pc
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
ENDIF()

OPTION(G3KBSWITCH_INSTALL_MAN_PAGE "Install man documentation" ON)
IF(G3KBSWITCH_INSTALL_MAN_PAGE)
    SET(man_page ${name}.1)
    FIND_PROGRAM(GZIP gzip)
    IF (GZIP)
        SET(man_gz ${CMAKE_BINARY_DIR}/${man_page}.gz)
        ADD_CUSTOM_TARGET(man-${name} ALL DEPENDS ${man_gz})
        ADD_CUSTOM_COMMAND(
            OUTPUT ${man_gz}
            COMMAND ${GZIP} -9 --no-name --to-stdout "${man_page}" >"${man_gz}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
        SET(man_page ${man_gz})
    ELSE()
        SET(man_page ${CMAKE_SOURCE_DIR}/docs/${man_page})
    ENDIF()
    INSTALL(FILES ${man_page}
            DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
ENDIF()

