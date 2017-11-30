if (USE_PACKAGE_CURL)
else ()
    include(ExternalProject)
    #include(cmake/deps/openssl.cmake)

    set(CURL_SOURCE_DIR ${CMAKE_BINARY_DIR}/src/curl CACHE INTERNAL "Source directory for Curl build")
    set(CURL_INSTALL_DIR ${THIRD_PARTY_INSTALL_DIR}/curl CACHE INTERNAL "Install directory for Curl")
    set(CURL_LIB_DIR ${CURL_INSTALL_DIR}/lib CACHE INTERNAL "Library directory for Curl")
    set(CURL_INCLUDE_DIR ${CURL_INSTALL_DIR}/include CACHE INTERNAL "Include directories for Curl")
    set(CURL_CONFIG_OPTIONS --disable-shared --disable-ldap --disable-ldaps --enable-threaded-resolver --disable-debug)
    list(APPEND CURL_CONFIG_OPTIONS --without-libssh2 --without-ca-bundle --without-libidn)
    list(APPEND CURL_CONFIG_OPTIONS --with-ssl=${OPENSSL_INSTALL_DIR})
    list(APPEND CURL_CONFIG_OPTIONS --with-zlib=${ZLIB_INSTALL_DIR})
    list(APPEND CURL_CONFIG_OPTIONS --prefix=${CURL_INSTALL_DIR})
    set(CURL_CONFIG_COMMAND ${CURL_SOURCE_DIR}/configure ${CURL_CONFIG_OPTIONS})

    include(cmake/deps/target-create.cmake)
    create_third_party_library(curl libcurl.a ${CURL_LIB_DIR} ${CURL_INCLUDE_DIR} CURL THIRD_PARTY_INCLUDES THIRD_PARTY_LIBS THIRD_PARTY_TARGETS)

    get_target_property(CURL_ARTIFACT curl IMPORTED_LOCATION)

    externalproject_add(
            CURL
            SOURCE_DIR ${CURL_SOURCE_DIR}
            URL https://curl.haxx.se/download/curl-7.57.0.tar.gz
            URL_HASH "SHA256=7ce35f207562674e71dbada6891b37e3f043c1e7a82915cb9c2a17ad3a9d659b"
            CONFIGURE_COMMAND ${CURL_SOURCE_DIR}/configure ${CURL_CONFIG_OPTIONS} ${THIRD_PARTY_CFLAGS} ${THIRD_PARTY_CXXFLAGS} ${THIRD_PARTY_LDFLAGS}
            BUILD_COMMAND make -j4
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND make install
            DEPENDS OPENSSL ZLIB
            BUILD_BYPRODUCTS ${CURL_ARTIFACT}
    )

    if (APPLE)
        set(CURL_MAC_SED_PATTERN "s/\#define HAVE_CLOCK_GETTIME_MONOTONIC 1//")
        externalproject_add_step(
                CURL
                MACOS_SIERRA_FIX
                DEPENDEES configure
                WORKING_DIRECTORY <SOURCE_DIR>
                COMMAND sed -Ei .bak ${CURL_MAC_SED_PATTERN} lib/curl_config.h
        )
    endif ()
    message(STATUS "Curl Install Dir: ${CURL_INSTALL_DIR}")


endif ()
