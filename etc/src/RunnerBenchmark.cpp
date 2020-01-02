//
//  Copyright (C) 2014-2020 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                <https://github.com/casm-lang/libcasm-tc>
//
//  This file is part of libcasm-tc.
//
//  libcasm-tc is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcasm-tc is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcasm-tc. If not, see <http://www.gnu.org/licenses/>.
//
//  Additional permission under GNU GPL version 3 section 7
//
//  libcasm-tc is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcasm-tc
//  statically or dynamically with other modules is making a combined work
//  based on libcasm-tc. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcasm-tc give you permission to link libcasm-tc
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcasm-tc. If you modify libcasm-tc, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "RunnerBenchmark.h"

#include "Profile.h"

#include <libstdhl/Environment>

#include <cassert>
#include <unordered_map>

void benchmark::SetUp()
{
    m_cmd[ 0 ] = '\0';

    libstdhl::u32 exec_result = 0;
    assert( exec_result == 0 );

    std::unordered_map< std::string, std::string > env;
    env[ "EXPORT" ] = "export";
    env[ "ECHO" ] = "echo";
    env[ "CAT" ] = "cat";
    env[ "CASM" ] = "";
    env[ "CASM_BM" ] = "";
    env[ "CASM_ARG_PRE" ] = "";
    env[ "CASM_ARG_POST" ] = "";

    if( not libstdhl::Environment::Variable::has( "CASM" ) or
        libstdhl::Environment::Variable::get( "CASM" ).length() == 0 )
    {
        printf( "\nenvironment variable 'CASM' not set, omitting benchmark!\n\n" );
        return;
    }

    for( auto& e : env )
    {
        if( libstdhl::Environment::Variable::has( e.first ) )
        {
            env[ e.first ] = libstdhl::Environment::Variable::get( e.first );
        }
    }

    std::string bm = "obj/.bm";
    sprintf( m_cmd, "%s -t > %s", env[ "CASM" ].c_str(), bm.c_str() );
    exec_result = system( m_cmd );
    assert( exec_result == 0 );

    FILE* BM = fopen( bm.c_str(), "r" );
    fgets( m_cmd, 4096, BM );
    libstdhl::Environment::Variable::set( "CASM_BM", m_cmd );
    fclose( BM );

    env[ "CASM_BM" ] = libstdhl::Environment::Variable::get( "CASM_BM" );
    assert( env[ "CASM_BM" ].length() > 0 );

    const auto testCaseProfile = libcasm_tc::Profile::fromString( env[ "CASM_BM" ] );
    switch( testCaseProfile )
    {
        case libcasm_tc::Profile::INTERPRETER:  // [[fallthrough]]
        case libcasm_tc::Profile::FORMAT:
        {
            sprintf(
                m_cmd,
                "%s %s %s %s 2>&1 > obj/.bm",
                env[ "CASM" ].c_str(),
                env[ "CASM_ARG_PRE" ].c_str(),
                "%s",
                env[ "CASM_ARG_POST" ].c_str() );
            break;
        }
        default:
        {
            assert( !" unsupported test case profile to benchmark! " );
        }
    }
}

void benchmark::run( const char* spec )
{
    char cmd[ 4096 ];
    sprintf( cmd, m_cmd, spec );
    assert( system( cmd ) == 0 );
}

void benchmark::TearDown()
{
}

// BENCHMARK_P_INSTANCE( benchmark, benchmark_name, ( "todo/benchmark/bubblesort.casm"
// ) );
