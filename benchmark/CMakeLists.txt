#
#   Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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
file( GLOB_RECURSE CASM_BENCHMARK_FILES "*.casm" )
set( CXX_BENCHMARK_FILES "" )
foreach( CASM_BENCHMARK_FILE
    ${CASM_BENCHMARK_FILES}
    )
  set( BENCHMARK_FILE "" )
  string( SUBSTRING ${CASM_BENCHMARK_FILE} ${CWD_LENGTH} -1 BENCHMARK_FILE )
  set( CXX_BENCHMARK_NAME ${BENCHMARK_FILE}.benchmark.cpp )
  set( CXX_BENCHMARK_FILE ${CMAKE_CURRENT_BINARY_DIR}/${CXX_BENCHMARK_NAME} )
  set( CXX_BENCHMARK_FILES "${CXX_BENCHMARK_FILES};${CXX_BENCHMARK_FILE}" )
  get_filename_component( CXX_BENCHMARK_DIR ${CXX_BENCHMARK_FILE} DIRECTORY )
  add_custom_command(
    OUTPUT  ${CXX_BENCHMARK_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CXX_BENCHMARK_DIR}
    COMMAND ${PROJECT}-generator benchmark ${CASM_BENCHMARK_FILE} ${CXX_BENCHMARK_FILE} ${CMAKE_SOURCE_DIR}/
    DEPENDS ${PROJECT}-generator ${CASM_BENCHMARK_FILE}
    COMMENT "Generating benchmark case ${BENCHMARK_FILE}"
    )
endforeach()

set( CXX_BENCHMARK ${CMAKE_CURRENT_BINARY_DIR}/cases.cpp )
add_custom_command(
  OUTPUT  ${CXX_BENCHMARK}
  COMMAND cat ${CXX_BENCHMARK_FILES} > ${CXX_BENCHMARK}
  DEPENDS ${CXX_BENCHMARK_FILES}
  COMMENT "Updating  benchmark cases ${CXX_BENCHMARK}"
  )


include_directories(
  ${PROJECT_BINARY_DIR}/etc/src
  ${LIBHAYAI_INCLUDE_DIR}
  ${LIBSTDHL_INCLUDE_DIR}
  )

add_library( ${PROJECT}-benchmark-cases OBJECT
  ${CXX_BENCHMARK}
  )
