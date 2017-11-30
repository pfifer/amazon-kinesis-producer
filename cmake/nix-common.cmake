set(PLATFORM_SPECIFIC_SOURCE aws/utils/openssl_md5_hasher.cc aws/kinesis/platform/nix.cc)
set(THIRD_PARTY_INSTALL_DIR ${CMAKE_BINARY_DIR}/third-party CACHE INTERNAL "Location for third party installations")

include(cmake/deps/zlib.cmake)
include(cmake/deps/openssl.cmake)
include(cmake/deps/boost.cmake)
include(cmake/deps/curl.cmake)
include(cmake/deps/protobuf.cmake)
include(cmake/deps/aws-cpp-sdk.cmake)

#CREATE_BOOST_LIB(atomic)
#CREATE_BOOST_LIB(chrono)
#CREATE_BOOST_LIB(filesystem)
#CREATE_BOOST_LIB(date_time)
#CREATE_BOOST_LIB(regex)
#CREATE_BOOST_LIB(thread)
#CREATE_BOOST_LIB(log)
#CREATE_BOOST_LIB(log_setup)
#CREATE_BOOST_LIB(random)
#CREATE_BOOST_LIB(prg_exec_monitor)
#CREATE_BOOST_LIB(unit_test_framework)



message(STATUS "Final Includes: ${THIRD_PARTY_INCLUDES}")