function(create_third_party_library LIBRARY_NAME LIBRARY_FILE LIBRARY_DIR INCLUDE_DIR DEPENDENT_NAME INCLUDE_TARGET LIB_TARGET DEPENDS_TARGET)
    add_library(${LIBRARY_NAME} UNKNOWN IMPORTED)
    add_dependencies(${LIBRARY_NAME} ${DEPENDENT_NAME})
    message(STATUS "Lib: ${LIBRARY_NAME}: ${DEPENDENT_NAME}")
    file(MAKE_DIRECTORY ${LIBRARY_DIR})
    file(MAKE_DIRECTORY ${INCLUDE_DIR})
    set_property(TARGET ${LIBRARY_NAME} PROPERTY IMPORTED_LOCATION ${LIBRARY_DIR}/${LIBRARY_FILE})
    set_property(TARGET ${LIBRARY_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INCLUDE_DIR})

    set(NEED_TO_ADD 1)

    string(FIND "${${INCLUDE_TARGET}}" ${INCLUDE_DIR} FOUND_INCLUDE_DIR)

    if (NOT ${FOUND_INCLUDE_DIR} EQUAL -1)
        set(NEED_TO_ADD 0)
    endif ()
    if (NEED_TO_ADD)
        set(LOCAL_INCLUDE ${${INCLUDE_TARGET}} ${INCLUDE_DIR})
        set(${INCLUDE_TARGET} ${LOCAL_INCLUDE} PARENT_SCOPE)

        set(LOCAL_LIB ${${LIB_TARGET}} ${LIBRARY_DIR}/${LIBRARY_FILE})
        set(${LIB_TARGET} ${LOCAL_LIB} PARENT_SCOPE)

        set(LOCAL_DEP ${${DEPENDS_TARGET}} ${DEPENDENT_NAME})
        set(${DEPENDS_TARGET} ${LOCAL_DEP} PARENT_SCOPE)
    endif ()
endfunction()