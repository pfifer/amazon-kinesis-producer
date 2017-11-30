if (USE_PROVIDED_PROTOBUF)
else ()
    include(ExternalProject)

    set(PROTOBUF_SOURCE_DIR ${CMAKE_BINARY_DIR}/src/protobuf CACHE INTERNAL "Source directory for Protobuf build")
    set(PROTOBUF_INSTALL_DIR ${THIRD_PARTY_INSTALL_DIR}/protobuf CACHE INTERNAL "Install directory for Protobuf")
    set(PROTOBUF_LIB_DIR ${PROTOBUF_INSTALL_DIR}/lib CACHE INTERNAL "Library directory for Protobuf")
    set(PROTOBUF_INCLUDE_DIR ${PROTOBUF_INSTALL_DIR}/include CACHE INTERNAL "Include directories for Protobuf")
    set(PROTOBUF_CONFIG_OPTIONS --disable-shared --with-zlib=${ZLIB_INSTALL_DIR})
    set(PROTO_GENERATED_DIR ${CMAKE_BINARY_DIR}/proto)

    include(cmake/deps/target-create.cmake)

    create_third_party_library(protobuf libprotobuf.a ${PROTOBUF_LIB_DIR} ${PROTOBUF_INCLUDE_DIR} PROTOBUF THIRD_PARTY_INCLUDES THIRD_PARTY_LIBS THIRD_PARTY_TARGETS)
    get_target_property(PROTO_ARTIFACT protobuf IMPORTED_LOCATION)

    create_third_party_library(kpl-protobuf libkpl-protobuf.a ${PROTO_GENERATED_DIR}/lib ${PROTO_GENERATED_DIR}/include PROTOBUF_GEN THIRD_PARTY_INCLUDES THIRD_PARTY_LIBS THIRD_PARTY_TARGETS)
    get_target_property(KPL_PROTO_ARTIFACT kpl-protobuf IMPORTED_LOCATION)

    externalproject_add(
            PROTOBUF
            SOURCE_DIR ${PROTOBUF_SOURCE_DIR}
            URL https://github.com/google/protobuf/releases/download/v3.5.0/protobuf-all-3.5.0.tar.gz
            URL_HASH "SHA256=5c3e0b369e767993969fdebcb3f44cbddd7fdd86644a926780daab46341d10a7"
            CONFIGURE_COMMAND ${PROTOBUF_SOURCE_DIR}/configure --enable-shared=no --prefix=${PROTOBUF_INSTALL_DIR} ${THIRD_PARTY_CFLAGS} ${THIRD_PARTY_CXXFLAGS} ${THIRD_PARTY_LDFLAGS}
            BUILD_COMMAND make -j4
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND make install
            DEPENDS ZLIB
            BUILD_BYPRODUCTS ${PROTO_ARTIFACT}
    )

    externalproject_add(
            PROTOBUF_GEN
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/cmake/gen-protobuf
            CMAKE_ARGS
            -DCMAKE_PREFIX_PATH=${PROTOBUF_INSTALL_DIR}
            -DPROTO_DEF_DIR=${CMAKE_SOURCE_DIR}/aws/kinesis/protobuf
            -DCMAKE_INSTALL_PREFIX=${PROTO_GENERATED_DIR}
            DEPENDS PROTOBUF
            BUILD_BYPRODUCTS ${KPL_PROTO_ARTIFACT}
    )

#    set(PROTO_FILES_DIR ${CMAKE_SOURCE_DIR}/aws/kinesis/protobuf)
#
#
#    file(GLOB PROTO_FILES ${PROTO_FILES_DIR}/*.proto)
#
#    file(MAKE_DIRECTORY ${PROTO_GENERATED_DIR})
#
#    externalproject_add_step(
#            PROTOBUF
#            compile-proto-files
#            DEPENDEES install
#            WORKING_DIRECTORY <INSTALL_DIR>
#            COMMAND ${PROTOBUF_INSTALL_DIR}/bin/protoc --proto_path=${PROTO_FILES_DIR} --cpp_out ${PROTO_GENERATED_DIR} ${PROTO_FILES}
#            DEPENDS ${PROTO_FILES}
#    )
#
#    file(GLOB GENERATED_PROTO_SRC ${PROTO_GENERATED_DIR}/*.c*)
#    file(GLOB GENERATED_PROTO_HEADERS ${PROTO_GENERATED_DIR}/*.h*)
#
#    set_source_files_properties(${GENERATED_PROTO_SRC} ${GENERATED_PROTO_HEADERS} PROPERTIES GENERATED TRUE)
    #add_library(KPL_PROTOBUF OBJECT ${GENERATED_PROTO_SRC})
    #target_include_directories(KPL_PROTOBUF ${GENERATED_PROTO_HEADERS})


endif ()
