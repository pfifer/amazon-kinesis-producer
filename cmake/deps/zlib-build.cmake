
find_package(zlib)
if (ZLIB_FOUND AND USE_LOCATED_ZLIB)
    message(STATUS "Found zlib ${ZLIB_LIBRARIES}, and configured to use located zlib")
else ()

    include(ExternalProject)

    set(ZLIB_SOURCE_DIR ${CMAKE_BINARY_DIR}/src/zlib CACHE INTERNAL "zlib source dir")
    set(ZLIB_INSTALL_DIR ${THIRD_PARTY_INSTALL_DIR} CACHE INTERNAL "zlib installation directory")
    set(ZLIB_LIB_DIR ${ZLIB_INSTALL_DIR}/lib CACHE INTERNAL "zlib library directory")
    set(ZLIB_INCLUDE_DIR ${ZLIB_INSTALL_DIR}/include CACHE INTERNAL "zlib include directory")


    externalproject_add(ZLIB
            SOURCE_DIR ${ZLIB_SOURCE_DIR}
            URL https://zlib.net/zlib-1.2.11.tar.gz
            URL_HASH "SHA256=c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1"
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${ZLIB_INSTALL_DIR}
            -DBUILD_SHARED_LIBS=0
            )

    add_library(zlib UNKNOWN IMPORTED)
    set_property(TARGET zlib PROPERTY IMPORTED_LOCATION ${ZLIB_LIB_DIR}/libz.a)
endif ()