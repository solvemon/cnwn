cmake_minimum_required (VERSION 3.2)
project(cnwn C)

include(GNUInstallDirs)
#include(ExternalProject)
include(TestBigEndian)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Debug release or empty." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE MATCHES "^(debug|release)$")
  message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_C_FLAGS "-std=c99 -Wall -pedantic")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-O2")

option(BUILD_TESTS "Build tests" ON)
option(BUILD_TOOLS "Build tools" ON)
option(BUILD_XML "Build XML support" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_definitions(-DBUILD_VERSION_MAJOR=${VERSION_MAJOR} -DBUILD_VERSION_MINOR=${VERSION_MINOR} -DBUILD_VERSION_PATCH=${VERSION_PATCH})
add_definitions(-D_POSIX_C_SOURCE=200112L)

test_big_endian(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
  add_definitions(-DBUILD_BIG_ENDIAN)
else()
  add_definitions(-DBUILD_LITTLE_ENDIAN)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-DBUILD_WINDOWS_FILE)
endif()

if (BUILD_XML)
  add_definitions(-DBUILD_XML)
endif()


set(LIBRARY_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/endian.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/string.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/path.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/hash.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/containers.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/regexp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/options.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource_type.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/erf.c
  )


add_library(cnwn-shared SHARED ${LIBRARY_SOURCE_FILES})
set_target_properties(cnwn-shared PROPERTIES COMPILE_DEFINITIONS BUILD_API OUTPUT_NAME "cnwn" SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
#target_link_libraries(cnwn-shared ${CMAKE_CURRENT_BINARY_DIR}/lib/libmxml.a)
#add_dependencies(cnwn-shared)
add_library(cnwn-static STATIC ${LIBRARY_SOURCE_FILES})
set_target_properties(cnwn-static PROPERTIES COMPILE_DEFINITIONS BUILD_API OUTPUT_NAME "cnwn" SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

if(BUILD_TOOLS)
  add_executable(cnwna src/cnwna.c)
  target_link_libraries(cnwna cnwn-static)
endif()

if(BUILD_TESTS)
  add_executable(test-string tests/test-string.c)
  target_link_libraries(test-string cnwn-shared)
  add_executable(test-path tests/test-path.c)
  target_link_libraries(test-path cnwn-shared)
  add_executable(test-file_system tests/test-file_system.c)
  target_link_libraries(test-file_system cnwn-shared)
  add_executable(test-options tests/test-options.c)
  target_link_libraries(test-options cnwn-shared)
  add_executable(test-resource tests/test-resource.c)
  target_link_libraries(test-resource cnwn-shared)
endif()

install(TARGETS cnwn-shared DESTINATION "${CMAKE_INSTALL_LIBDIR}")
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cnwn.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
  set(DOXYGEN_BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg)
  add_custom_target(
    docs
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen.cfg
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
else (DOXYGEN_FOUND)
  message("No doxygen = no docs!")
endif (DOXYGEN_FOUND)

