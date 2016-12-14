#
#   Copyright (c) 2014-2016 CASM Organization
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 https://github.com/casm-lang/libcasm-tc
#
#   This file is part of libcasm-tc.
#
#   libcasm-tc is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   libcasm-tc is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with libcasm-tc. If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required( VERSION 3.6 )

if( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../gtest" )
  set( LIBS .. )
else()
  set( LIBS lib )
endif()

foreach( DEPENDENCY
    stdhl
    )
  if( NOT TARGET ${DEPENDENCY} )
    add_subdirectory( ${LIBS}/${DEPENDENCY} lib/${DEPENDENCY} )
  endif()  
endforeach()


file( GLOB_RECURSE CXX_SRC_FILES "src/*.cpp" )
file( GLOB_RECURSE CASM_UTS_FILES "uts/*.casm" )


set( TARGET casm-tc )
project( ${TARGET} )


include_directories(
  .
  src
  #uts
  ${LIBS}/gtest/googletest
  ${LIBS}/gtest/googletest/include
  ${LIBS}/hayai/src
  ${LIBS}/stdhl
)


set( CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS}          -std=c++11 -Wall" )
set( CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}    -O0" )
set( CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE}  -O3" )
set( CMAKE_CXX_FLAGS_SANITIZE "${CMAKE_CXX_FLAGS_SANITIZE} -O1 -Wextra -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=undefined -fsanitize=address" )


string( LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/" CWD_LENGTH )


set( CXX_UTS_TC_FILES "" )
set( CXX_UTS_BM_FILES "" )

foreach( CASM_UTS_FILE
    ${CASM_UTS_FILES}
    )
  string( SUBSTRING ${CASM_UTS_FILE} ${CWD_LENGTH} -1 UTS_FILE )

  set( CXX_UTS_TC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/obj/${UTS_FILE}.tc.cpp )
  set( CXX_UTS_TC_FILES "${CXX_UTS_TC_FILES};${CXX_UTS_TC_FILE}" )

  set( CXX_UTS_BM_FILE ${CMAKE_CURRENT_SOURCE_DIR}/obj/${UTS_FILE}.bm.cpp )
  set( CXX_UTS_BM_FILES "${CXX_UTS_BM_FILES};${CXX_UTS_BM_FILE}" )

  add_custom_command(
    OUTPUT  ${CXX_UTS_TC_FILE} ${CXX_UTS_BM_FILE}
    COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR} obj/${UTS_FILE}.tc.cpp
    COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR} obj/${UTS_FILE}.bm.cpp
    DEPENDS ${TARGET} ${TARGET}-bm ${CASM_UTS_FILE}
    COMMENT "Generating test case ${UTS_FILE}"
    )
endforeach()


set( CXX_UTS_TEST ${CMAKE_CURRENT_SOURCE_DIR}/obj/uts_test.cpp )
add_custom_command(
  OUTPUT  ${CXX_UTS_TEST}
  COMMAND cat ${CXX_UTS_TC_FILES} > ${CXX_UTS_TEST}
  DEPENDS ${CXX_UTS_TC_FILES}
  COMMENT "Generating test check ${CXX_UTS_TEST}"
)

set( CXX_UTS_BENCHMARK ${CMAKE_CURRENT_SOURCE_DIR}/obj/uts_benchmark.cpp )
add_custom_command(
  OUTPUT  ${CXX_UTS_BENCHMARK}
  COMMAND cat ${CXX_UTS_BM_FILES} > ${CXX_UTS_BENCHMARK}
  DEPENDS ${CXX_UTS_BM_FILES}
  COMMENT "Generating benchmark  ${CXX_UTS_BENCHMARK}"
)


add_executable( ${TARGET}
  src/tc.cpp
)

target_link_libraries( ${TARGET}
  stdhl
)

add_dependencies( ${TARGET} ${TARGET}-bm )

add_library( ${TARGET}-test OBJECT
  ${CXX_UTS_TEST}
  uts/RunnerTest.cpp
)
  
add_executable( ${TARGET}-check
  ${LIBS}/gtest/googletest/src/gtest_main.cc
  ${LIBS}/gtest/googletest/src/gtest-all.cc  
  $<TARGET_OBJECTS:${TARGET}-test>
)

target_link_libraries( ${TARGET}-check
#   stdhl
  pthread
)



add_executable( ${TARGET}-bm
  src/bm.cpp
)

target_link_libraries( ${TARGET}-bm
  stdhl
)


add_library( ${TARGET}-benchmark OBJECT
  ${CXX_UTS_BENCHMARK}
  uts/RunnerBenchmark.cpp
)
  
add_executable( ${TARGET}-run
  ${LIBS}/hayai/src/hayai_posix_main.cpp
  $<TARGET_OBJECTS:${TARGET}-benchmark>
)

target_link_libraries( ${TARGET}-run
#   stdhl
  pthread
)


if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug
    CACHE STRING "Choose the type of build : debug sanitize release."
    FORCE
  )
endif( NOT CMAKE_BUILD_TYPE )


message( "* ${CMAKE_BUILD_TYPE} : ${TARGET} : ${LIBS}" )
