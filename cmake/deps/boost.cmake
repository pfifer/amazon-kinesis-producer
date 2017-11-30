#find_package(Boost COMPONENTS regex thread log system random filesystem chrono program_options unit_test_framework)

if (BOOST_FOUND AND USE_LOCATED_BOOST)
else ()
    include(ExternalProject)
    set(BOOST_SOURCE_DIR ${CMAKE_BINARY_DIR}/src/boost CACHE INTERNAL "Source directory for Boost build")
    set(BOOST_INSTALL_DIR ${THIRD_PARTY_INSTALL_DIR}/boost CACHE INTERNAL "Install directory for Boost")
    set(BOOST_LIB_DIR ${BOOST_INSTALL_DIR}/lib CACHE INTERNAL "Library directory for Boost")
    set(BOOST_INCLUDE_DIR ${BOOST_INSTALL_DIR}/include CACHE INTERNAL "Library directory for Boost")
    set(BOOST_BUILD_LIBS atomic,chrono,log,system,test,random,regex,thread,filesystem,program_options)
    set(BOOST_BUILD_OPTIONS -j 8 --build-type=minimal --layout=system --prefix=${BOOST_INSTALL_DIR} link=static threading=multi ${THIRD_PARTY_CXXFLAGS} ${THIRD_PARTY_LDFLAGS} release)

    set(BOOST_LIB_NAMES atomic chrono filesystem date_time regex thread log log_setup random program_options system)
    set(BOOST_TEST_LIB_NAMES prg_exec_monitor unit_test_framework)
    include(cmake/deps/target-create.cmake)
    macro(CREATE_BOOST_LIB BOOST_NAME)
        create_third_party_library(boost_${BOOST_NAME} libboost_${BOOST_NAME}.a ${BOOST_LIB_DIR} ${BOOST_INCLUDE_DIR} BOOST THIRD_PARTY_INCLUDES THIRD_PARTY_LIBS THIRD_PARTY_TARGETS)
    endmacro()

    CREATE_BOOST_LIB(atomic)
    CREATE_BOOST_LIB(chrono)
    CREATE_BOOST_LIB(filesystem)
    CREATE_BOOST_LIB(date_time)
    CREATE_BOOST_LIB(regex)
    CREATE_BOOST_LIB(thread)
    CREATE_BOOST_LIB(log)
    CREATE_BOOST_LIB(log_setup)
    CREATE_BOOST_LIB(random)
    CREATE_BOOST_LIB(program_options)
    CREATE_BOOST_LIB(system)
    create_third_party_library(boost_prg_exec_monitor libboost_prg_exec_monitor.a ${BOOST_LIB_DIR} ${BOOST_INCLUDE_DIR} BOOST TEST_THIRD_PARTY_INCLUDES TEST_THIRD_PARTY_LIBS TEST_THIRD_PARTY_TARGETS)
    create_third_party_library(boost_unit_test_framework libboost_unit_test_framework.a ${BOOST_LIB_DIR} ${BOOST_INCLUDE_DIR} BOOST TEST_THIRD_PARTY_INCLUDES TEST_THIRD_PARTY_LIBS TEST_THIRD_PARTY_TARGETS)

    add_dependencies(boost_log boost_log_setup)
    add_dependencies(boost_unit_test_framework prg_exec_monitor)

    foreach (BOOST_LIB_NAME ${BOOST_LIB_NAMES} ${BOOST_TEST_LIB_NAMES})
        get_target_property(BOOST_LIB_ARTIFACT boost_${BOOST_LIB_NAME} IMPORTED_LOCATION)
        list(APPEND BOOST_ARTIFACTS ${BOOST_LIB_ARTIFACT})
    endforeach ()

    externalproject_add(
            BOOST
            SOURCE_DIR ${BOOST_SOURCE_DIR}
            URL https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz
            URL_HASH "SHA256=a13de2c8fbad635e6ba9c8f8714a0e6b4264b60a29b964b940a22554705b6b60"
            CONFIGURE_COMMAND ${BOOST_SOURCE_DIR}/bootstrap.sh --with-libraries=${BOOST_BUILD_LIBS}
            BUILD_COMMAND ${BOOST_SOURCE_DIR}/b2 ${BOOST_BUILD_OPTIONS}
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND ${BOOST_SOURCE_DIR}/b2 --prefix=${BOOST_INSTALL_DIR} install
            BUILD_BYPRODUCTS ${BOOST_ARTIFACTS}
    )
    message(STATUS "Boost Install Dir: ${OPENSSL_INSTALL_DIR}")


endif ()
