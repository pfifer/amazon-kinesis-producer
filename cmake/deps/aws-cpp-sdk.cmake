if (USE_PROVIDED_AWS_CPP_SDK)
else ()
    include(ExternalProject)

    set(AWS_CPP_SDK_SOURCE_DIR ${CMAKE_BINARY_DIR}/src/aws-sdk-cpp CACHE INTERNAL "Source directory for aws-cpp-sdk build")
    set(AWS_CPP_SDK_INSTALL_DIR ${THIRD_PARTY_INSTALL_DIR}/aws-sdk-cpp CACHE INTERNAL "Install directory for aws-cpp-sdk")
    set(AWS_CPP_SDK_LIB_DIR ${AWS_CPP_SDK_INSTALL_DIR}/lib CACHE INTERNAL "Library directory for aws-cpp-sdk")
    set(AWS_CPP_SDK_INCLUDE_DIR ${AWS_CPP_SDK_INSTALL_DIR}/include CACHE INTERNAL "Include directories for aws-cpp-sdk")
    set(AWS_CPP_SDK_CONFIG_OPTIONS --disable-shared --with-zlib=${ZLIB_INSTALL_DIR})

    include(cmake/deps/target-create.cmake)

    create_third_party_library(aws-cpp-sdk-core libaws-cpp-sdk-core.a ${AWS_CPP_SDK_LIB_DIR} ${AWS_CPP_SDK_INCLUDE_DIR} AWS_CPP_SDK THIRD_PARTY_INCLUDES THIRD_PARTY_LIBS THIRD_PARTY_TARGETS)
    add_dependencies(aws-cpp-sdk-core curl)

    create_third_party_library(aws-cpp-sdk-kinesis libaws-cpp-sdk-kinesis.a ${AWS_CPP_SDK_LIB_DIR} ${AWS_CPP_SDK_INCLUDE_DIR} AWS_CPP_SDK THIRD_PARTY_INCLUDES THIRD_PARTY_LIBS THIRD_PARTY_TARGETS)
    add_dependencies(aws-cpp-sdk-kinesis aws-cpp-sdk-core)

    create_third_party_library(aws-cpp-sdk-monitoring libaws-cpp-sdk-monitoring.a ${AWS_CPP_SDK_LIB_DIR} ${AWS_CPP_SDK_INCLUDE_DIR} AWS_CPP_SDK THIRD_PARTY_INCLUDES THIRD_PARTY_LIBS THIRD_PARTY_TARGETS)
    add_dependencies(aws-cpp-sdk-monitoring aws-cpp-sdk-core)

    foreach (SDK_LIB aws-cpp-sdk-core aws-cpp-sdk-kinesis aws-cpp-sdk-monitoring)
        get_target_property(SDK_LIB_ARTIFACT ${SDK_LIB} IMPORTED_LOCATION)
        list(APPEND SDK_ARTIFACTS ${SDK_LIB_ARTIFACT})
    endforeach ()


    externalproject_add(
            AWS_CPP_SDK
            SOURCE_DIR ${AWS_CPP_SDK_SOURCE_DIR}
            GIT_REPOSITORY https://github.com/aws/aws-sdk-cpp.git
            GIT_TAG 1.3.12
            LIST_SEPARATOR ,
            CMAKE_ARGS
            -DBUILD_ONLY=kinesis,monitoring
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DBUILD_SHARED_LIBS=OFF
            -DZLIB_ROOT=${ZLIB_INSTALL_DIR}
            -DCMAKE_PREFIX_PATH=${CURL_INSTALL_DIR}
            -DCMAKE_FIND_FRAMEWORK=LAST
            -DENABLE_TESTING=OFF
            -DCMAKE_INSTALL_PREFIX=${AWS_CPP_SDK_INSTALL_DIR}
            -DBUILD_SHARED_LIBS=0
            -DCUSTOM_MEMORY_MANAGEMENT=0
            DEPENDS CURL
            BUILD_BYPRODUCTS ${SDK_ARTIFACTS}
    )

    add_custom_target(aws-sdk-cpp)
    add_dependencies(aws-sdk-cpp aws-cpp-sdk-kinesis aws-cpp-sdk-monitoring)



endif ()
