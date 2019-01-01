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

# LIBCASM_TC_FOUND        - system has found the package
# LIBCASM_TC_INCLUDE_DIR  - the package include directory
# LIBCASM_TC_LIBRARY      - the package library
# LIBCASM_TC_TEST         - the test case library
# LIBCASM_TC_BENCHMARK    - the benchmark library

include( LibPackage )

libfind_pkg_check_modules( LIBCASM_TC_PKGCONF libcasm-tc )

find_path( LIBCASM_TC_INCLUDE_DIR
  NAMES libcasm-tc/libcasm-tc.h
  PATHS ${LIBCASM_TC_PKGCONF_INCLUDE_DIRS}
)

find_library( LIBCASM_TC_LIBRARY
  NAMES libcasm-tc casm-tc
  PATHS ${LIBCASM_TC_PKGCONF_LIBRARY_DIRS}
)

find_library( LIBCASM_TC_TEST
  NAMES libcasm-tc-test casm-tc-test
  PATHS ${LIBCASM_TC_PKGCONF_LIBRARY_DIRS}
)

find_library( LIBCASM_TC_BENCHMARK
  NAMES libcasm-tc-benchmark casm-tc-benchmark
  PATHS ${LIBCASM_TC_PKGCONF_LIBRARY_DIRS}
)

set( LIBCASM_TC_PROCESS_INCLUDES LIBCASM_TC_INCLUDE_DIR )
set( LIBCASM_TC_PROCESS_LIBS     LIBCASM_TC_LIBRARY )
set( LIBCASM_TC_TEST             ${LIBCASM_TC_TEST} PARENT_SCOPE )
set( LIBCASM_TC_BENCHMARK        ${LIBCASM_TC_BENCHMARK} PARENT_SCOPE )

libfind_process( LIBCASM_TC )

if( EXISTS "${LIBCASM_TC_LIBRARY}"   AND ${LIBCASM_TC_LIBRARY} AND
    EXISTS "${LIBCASM_TC_TEST}"      AND ${LIBCASM_TC_TEST}    AND
    EXISTS "${LIBCASM_TC_BENCHMARK}" AND ${LIBCASM_TC_BENCHMARK} )
  set( LIBCASM_TC_FOUND TRUE PARENT_SCOPE )
else()
  set( LIBCASM_TC_FOUND FALSE PARENT_SCOPE )
endif()
