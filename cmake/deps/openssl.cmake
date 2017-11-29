find_package(OpenSSL)

if (OPENSSL_FOUND AND USE_LOCATED_OPENSSL)
else ()
    include(ExternalProject)
    set(OPENSSL_SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl CACHE INTERNAL "Source directory for OpenSSL build")
    set(OPENSSL_INSTALL_DIR ${THIRD_PARTY_INSTALL_DIR}/openssl CACHE INTERNAL "Install directory for OpenSSL")
    set(OPENSSL_LIB_DIR ${OPENSSL_INSTALL_DIR}/lib CACHE INTERNAL "Library directory for OpenSSL")
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_DIR}/include CACHE INTERNAL "Include directories for OpenSSL")
    set(OPENSSL_CONFIG_OPTIONS "threads no-shared no-idea no-camellia no-seed no-bf no-cast no-rc2 no-rc4 no-rc5 no-md2 no-mdc2 no-ssl3 no-capieng no-dso")
    set(OPENSSL_CONFIG_COMMAND "${OPENSSL_SOURCE_DIR}/config ${OPENSSL_CONFIG_OPTIONS} --prefix=${OPENSSL_INSTALL_DIR}")

#    set(OPENSSL_SCRIPT ${CMAKE_BINARY_DIR}/configure_openssl.sh)
#    file(WRITE ${OPENSSL_SCRIPT} "#!/bin/sh\n")
#    file(APPEND ${OPENSSL_SCRIPT} "export ${THIRD_PARTY_CFLAGS}\n")
#    file(APPEND ${OPENSSL_SCRIPT} "export ${THIRD_PARTY_CXXFLAGS}\n")
#    #file(APPEND ${OPENSSL_SCRIPT} "cd ${OPENSSL_SOURCE_DIR}")
#    if (APPLE)
#        file(APPEND ${OPENSSL_SCRIPT} "${OPENSSL_SOURCE_DIR}/Configure darwin64-x86_64-cc ${OPENSSL_CONFIG_OPTIONS} --prefix=${OPENSSL_INSTALL_DIR}")
#    else ()
#        file(APPEND ${OPENSSL_SCRIPT} "")
#    endif ()
#
#    message(STATUS "Script file: ${OPENSSL_SCRIPT}")

    externalproject_add(
            OPENSSL
            SOURCE_DIR ${OPENSSL_SOURCE_DIR}
            URL https://www.openssl.org/source/openssl-1.1.0g.tar.gz
            URL_HASH "SHA256=de4d501267da39310905cb6dc8c6121f7a2cad45a7707f76df828fe1b85073af"
            PATCH_COMMAND patch -p1 < ${CMAKE_SOURCE_DIR}/openssl.patch
            CONFIGURE_COMMAND ${OPENSSL_SOURCE_DIR}/config ${OPENSSL_CONFIG_OPTIONS} --prefix=${OPENSSL_INSTALL_DIR}
            BUILD_COMMAND make
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND make install
            LOG_INSTALL 0
    )
    message(STATUS "OpenSSL Install Dir: ${OPENSSL_INSTALL_DIR}")

    include(cmake/deps/target-create.cmake)

    create_third_party_library(ssl libssl.a ${OPENSSL_LIB_DIR} ${OPENSSL_INCLUDE_DIR})
    create_third_party_library(crypto libcrypto.a ${OPENSSL_LIB_DIR} ${OPENSSL_INCLUDE_DIR})

endif ()