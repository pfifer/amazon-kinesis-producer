project(amazon-kinesis-producer-generate-protobuf)
cmake_minimum_required(VERSION 3.7)

option(PROTO_DEF_DIR "Directory containing the protobuf definition files")

find_package(Protobuf REQUIRED)

file(GLOB PROTO_DEF_FILES ${PROTO_DEF_DIR}/*.proto)
# aws/kinesis/protobuf/messages.proto aws/kinesis/protobuf/config.proto
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_DEF_FILES})
#set(GEN_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
#file(MAKE_DIRECTORY ${GEN_INCLUDE_DIR})
#message(STATUS "Proto Headers: ${PROTO_HDRS}")
#foreach (proto_header ${PROTO_HDRS})
#    file(RENAME ${proto_header} ${GEN_INCLUDE_DIR}/)
#endforeach ()
#file(GLOB ALL_HEADERS ${GEN_INCLUDE_DIR}/*.h)
add_library(kpl-protobuf STATIC ${PROTO_SRCS} ${PROTO_HDRS})
include_directories(kpl-protobuf ${Protobuf_INCLUDE_DIR})

install(TARGETS kpl-protobuf
        ARCHIVE DESTINATION lib)
install(FILES ${PROTO_HDRS}
        DESTINATION include)