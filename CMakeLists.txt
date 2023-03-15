cmake_minimum_required(VERSION 3.9)

project("ufbx" LANGUAGES C CXX VERSION 0.16.99)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_SHARED_LIBS  "Default to building shared libraries" ON)
option(BUILD_STATIC_LIBS  "Default to building static libraries" ON)

set(JSON_C_PUBLIC_HEADERS
    ${PROJECT_SOURCE_DIR}/ufbx.h
)

set(JSON_C_HEADERS
    ${JSON_C_PUBLIC_HEADERS}
)

set(JSON_C_SOURCES
    ${PROJECT_SOURCE_DIR}/ufbx.c
)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})

# XXX for a normal full distribution we'll need to figure out
# XXX how to build both shared and static libraries.
# Probably leverage that to build a local VALGRIND=1 library for testing too.
add_library(${PROJECT_NAME}
    ${JSON_C_SOURCES}
    ${JSON_C_HEADERS}
)
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     VERSION 5.2.0
#     SOVERSION 5)

list(APPEND CMAKE_TARGETS ${PROJECT_NAME})
# If json-c is used as subroject it set to target correct interface -I flags and allow
# to build external target without extra include_directories(...)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_REQUIRED_LIBRARIES})

# Allow to build static and shared libraries at the same time
if (BUILD_STATIC_LIBS AND BUILD_SHARED_LIBS)
    set(STATIC_LIB ${PROJECT_NAME}-static)
    add_library(${STATIC_LIB} STATIC
        ${JSON_C_SOURCES}
        ${JSON_C_HEADERS}
    )
    target_include_directories(${PROJECT_NAME}-static
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    )

	target_link_libraries(${PROJECT_NAME}-static PUBLIC ${CMAKE_REQUIRED_LIBRARIES})

    # rename the static library
    if (NOT MSVC)
    set_target_properties(${STATIC_LIB} PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
    )
    endif()
    list(APPEND CMAKE_TARGETS ${STATIC_LIB})
endif ()

# Always create new install dirs with 0755 permissions, regardless of umask
set(CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
	OWNER_READ
	OWNER_WRITE
	OWNER_EXECUTE
	GROUP_READ
	GROUP_EXECUTE
	WORLD_READ
	WORLD_EXECUTE
   )

install(TARGETS ${CMAKE_TARGETS}
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} ${CMAKE_INSTALL_INCLUDEDIR}/ufbx
)

install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

configure_package_config_file(
    "cmake/config.cmake.in"
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
    FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

if (UNIX OR MINGW OR CYGWIN)
    SET(prefix ${CMAKE_INSTALL_PREFIX})
    # exec_prefix is prefix by default and CMake does not have the
    # concept.
    SET(exec_prefix ${CMAKE_INSTALL_PREFIX})
    SET(libdir ${CMAKE_INSTALL_FULL_LIBDIR})
    SET(includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})
    SET(VERSION ${PROJECT_VERSION})

	# Linking against the static json-c requires
	# dependent packages to include additional libs:
	SET(LIBS_LIST ${CMAKE_REQUIRED_LIBRARIES})

	# Note: We would need cmake >= 3.12 in order to use list(TRANSFORM ...)
	function(list_transform_prepend var prefix)
		set(temp "")
		foreach(f ${${var}})
			list(APPEND temp "${prefix}${f}")
		endforeach()
		set(${var} "${temp}" PARENT_SCOPE)
	endfunction()
	list_transform_prepend(LIBS_LIST "-l")

	string(REPLACE ";" " " LIBS "${LIBS_LIST}")
endif ()

install(FILES ${JSON_C_PUBLIC_HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/ufbx)