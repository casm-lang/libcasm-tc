#
#   Copyright (C) 2014-2021 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 <https://github.com/casm-lang/libcasm-tc>
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
#   Additional permission under GNU GPL version 3 section 7
#
#   libcasm-tc is distributed under the terms of the GNU General Public License
#   with the following clarification and special exception: Linking libcasm-tc
#   statically or dynamically with other modules is making a combined work
#   based on libcasm-tc. Thus, the terms and conditions of the GNU General
#   Public License cover the whole combination. As a special exception,
#   the copyright holders of libcasm-tc give you permission to link libcasm-tc
#   with independent modules to produce an executable, regardless of the
#   license terms of these independent modules, and to copy and distribute
#   the resulting executable under terms of your choice, provided that you
#   also meet, for each linked independent module, the terms and conditions
#   of the license of that module. An independent module is a module which
#   is not derived from or based on libcasm-tc. If you modify libcasm-tc, you
#   may extend this exception to your version of the library, but you are
#   not obliged to do so. If you do not wish to do so, delete this exception
#   statement from your version.
#

string( LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/" CWD_LENGTH )
file( GLOB_RECURSE CASM_TEST_FILES "*.casm" )
set( CXX_TEST_FILES "" )
set( CXX_TEST_CASES ${CMAKE_CURRENT_BINARY_DIR}/cases.txt )
file( WRITE ${CXX_TEST_CASES} "" )
foreach( CASM_TEST_FILE
    ${CASM_TEST_FILES}
    )
  set( TEST_FILE "" )
  string( SUBSTRING ${CASM_TEST_FILE} ${CWD_LENGTH} -1 TEST_FILE )
  set( CXX_TEST_NAME ${TEST_FILE}.test.cpp )
  set( CXX_TEST_FILE ${CMAKE_CURRENT_BINARY_DIR}/${CXX_TEST_NAME} )
  set( CXX_TEST_FILES "${CXX_TEST_FILES};${CXX_TEST_FILE}" )
  file( APPEND ${CXX_TEST_CASES} "${CXX_TEST_FILE}\n")

  get_filename_component( CXX_TEST_DIR ${CXX_TEST_FILE} DIRECTORY )
  add_custom_command(
    OUTPUT  ${CXX_TEST_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CXX_TEST_DIR}
    COMMAND ${PROJECT}-generator test ${CASM_TEST_FILE} ${CXX_TEST_FILE} ${CMAKE_SOURCE_DIR}/
    DEPENDS ${PROJECT}-generator ${CASM_TEST_FILE}
    COMMENT "Generating test case ${TEST_FILE}"
    )
endforeach()

set( CXX_TEST ${CMAKE_CURRENT_BINARY_DIR}/cases.cpp )
add_custom_command(
  OUTPUT  ${CXX_TEST}
  COMMAND make cases-test
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS ${CXX_TEST_FILES}
  COMMENT "Updating  test cases ${CXX_TEST}"
  )


include_directories(
  ${PROJECT_BINARY_DIR}/etc/src
  ${LIBGTEST_INCLUDE_DIR}
  ${LIBSTDHL_INCLUDE_DIR}
  )

add_library( ${PROJECT}-test-cases OBJECT
  ${CXX_TEST}
  )
