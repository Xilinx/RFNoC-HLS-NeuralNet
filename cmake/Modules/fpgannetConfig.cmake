INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_FPGANNET fpgannet)

FIND_PATH(
    FPGANNET_INCLUDE_DIRS
    NAMES fpgannet/api.h
    HINTS $ENV{FPGANNET_DIR}/include
        ${PC_FPGANNET_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    FPGANNET_LIBRARIES
    NAMES gnuradio-fpgannet
    HINTS $ENV{FPGANNET_DIR}/lib
        ${PC_FPGANNET_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FPGANNET DEFAULT_MSG FPGANNET_LIBRARIES FPGANNET_INCLUDE_DIRS)
MARK_AS_ADVANCED(FPGANNET_LIBRARIES FPGANNET_INCLUDE_DIRS)

