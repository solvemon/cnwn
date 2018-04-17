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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_definitions(-DVERSION_MAJOR=${VERSION_MAJOR} -DVERSION_MINOR=${VERSION_MINOR} -DVERSION_PATCH=${VERSION_PATCH} -D_POSIX_C_SOURCE=200112L)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
  add_definitions(-DBIG_ENDIAN)
endif()

set(LIBRARY_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/regulexp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/file.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resource.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/erf.c
  )


add_library(cnwn-shared SHARED ${LIBRARY_SOURCE_FILES})
set_target_properties(cnwn-shared PROPERTIES COMPILE_DEFINITIONS BUILD_API OUTPUT_NAME "cnwn" SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
#target_link_libraries(cnwn-shared ${CMAKE_CURRENT_BINARY_DIR}/lib/libmxml.a)
#add_dependencies(cnwn-shared)
add_library(cnwn-static STATIC ${LIBRARY_SOURCE_FILES})
set_target_properties(cnwn-static PROPERTIES COMPILE_DEFINITIONS BUILD_API OUTPUT_NAME "cnwn" SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
# target_link_libraries(cnwn-static ${OPENSSL_LIBRARIES})

# add_executable(cnwn-erf src/cnwn-erf.c)
# target_link_libraries(cnwn-erf cnwn-static)

if(BUILD_TESTS)
  add_executable(test-file tests/test-file.c)
  target_link_libraries(test-file cnwn-shared)
  add_executable(test-resource tests/test-resource.c)
  target_link_libraries(test-resource cnwn-shared)
  add_executable(test-erf tests/test-erf.c)
  target_link_libraries(test-erf cnwn-shared)
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

