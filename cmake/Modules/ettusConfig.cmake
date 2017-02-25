INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_ETTUS ettus)

FIND_PATH(
    ETTUS_INCLUDE_DIRS
    NAMES ettus/api.h
    HINTS $ENV{ETTUS_DIR}/include
        ${PC_ETTUS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    ETTUS_LIBRARIES
    NAMES gnuradio-ettus
    HINTS $ENV{ETTUS_DIR}/lib
        ${PC_ETTUS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ETTUS DEFAULT_MSG ETTUS_LIBRARIES ETTUS_INCLUDE_DIRS)
MARK_AS_ADVANCED(ETTUS_LIBRARIES ETTUS_INCLUDE_DIRS)

