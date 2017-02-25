INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_MACHLEARN machlearn)

FIND_PATH(
    MACHLEARN_INCLUDE_DIRS
    NAMES machlearn/api.h
    HINTS $ENV{MACHLEARN_DIR}/include
        ${PC_MACHLEARN_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    MACHLEARN_LIBRARIES
    NAMES gnuradio-machlearn
    HINTS $ENV{MACHLEARN_DIR}/lib
        ${PC_MACHLEARN_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MACHLEARN DEFAULT_MSG MACHLEARN_LIBRARIES MACHLEARN_INCLUDE_DIRS)
MARK_AS_ADVANCED(MACHLEARN_LIBRARIES MACHLEARN_INCLUDE_DIRS)

