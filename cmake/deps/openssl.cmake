find_package(OpenSSL)

if (OPENSSL_FOUND AND USE_LOCATED_OPENSSL)
else ()
    set(OPENSSL_SOURCE_DIR ${CMAKE_BINARY_DIR}/src/openssl CACHE INTERNAL "Source directory for OpenSSL build")
    set(OPENSSL_INSTALL_DIR ${THIRD_PARTY_INSTALL_DIR} CACHE INTERNAL "Install directory for OpenSSL")
    set(OPENSSL_LIB_DIR ${OPENSSL_INSTALL_DIR}/lib CACHE INTERNAL "Library directory for OpenSSL")
    set(OPENSSL_CONFIG_OPTIONS "threads no-shared no-idea no-camellia no-seed no-bf no-cast no-rc2 no-rc4 no-rc5 no-md2 no-mdc2 no-ssl3 no-capieng no-dso")
    set(OPENSSL_CONFIG_COMMAND "${OPENSSL_SOURCE_DIR}/config ${OPENSSL_CONFIG_OPTIONS} --prefix=${OPENSSL_INSTALL_DIR}")

    externalproject_add(
            OPENSSL
            SOURCE_DIR ${OPENSSL_SOURCE_DIR}
            URL https://www.openssl.org/source/openssl-1.1.0g.tar.gz
            URL_HASH "SHA256=de4d501267da39310905cb6dc8c6121f7a2cad45a7707f76df828fe1b85073af"
            CONFIGURE_COMMAND ${OPENSSL_SOURCE_DIR}/config ${OPENSSL_CONFIG_OPTIONS} --prefix=${OPENSSL_INSTALL_DIR}
            BUILD_COMMAND make
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND make install
    )
        message(STATUS "OpenSSL Install Dir: ${OPENSSL_INSTALL_DIR}")
    #find_package(OpenSSL PATHS ${OPENSSL_INSTALL_DIR})

endif ()