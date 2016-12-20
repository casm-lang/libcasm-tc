//
//  Copyright (c) 2014-2016 CASM Organization
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/casm-lang/libcasm-tc
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

#include "libstdhlcpp.h"

int main( int argc, const char* argv[] )
{
    assert( argc == 4 );

    assert( argv[ 1 ] );
    std::string mode( argv[ 1 ] );

    const char* file_name = argv[ 2 ];
    const char* dest_name = argv[ 3 ];

    // if( ( mode.compare( "tc" ) != 0 ) or ( mode.compare( "bm" ) != 0 ) )
    // {
    //     libstdhl::Log::error(
    //         "incorrect mode '%s' provided, only 'tc' or 'bm' are valid",
    //         mode.c_str() );
    //     assert( 0 );
    // }

    if( not libstdhl::File::exists( file_name ) )
    {
        libstdhl::Log::error( "unable to open file '%s'", file_name );
        assert( 0 );
    }

    std::vector< std::string > cmd;

    u1 no_cmd_found = true;

    struct ErrorInfo
    {
        std::string line;
        std::string code;
    };

    std::vector< ErrorInfo > error;

    libstdhl::File::readLines( file_name, [mode, file_name, dest_name,
                                              &no_cmd_found, &error]( u32 cnt,
                                              const std::string& line ) {
        std::string c = "";
        std::regex expr( "//@[ ]*([\\S]+)[ ]*\\([ ]*([\\S]*)[ ]*\\)" );
        std::sregex_iterator start( line.begin(), line.end(), expr );
        std::sregex_iterator end;

        for( std::sregex_iterator i = start; i != end; i++ )
        {
            std::smatch match = *i;
            std::string mstr = match.str();
            assert( match.size() == 3 );

            std::string func = match[ 1 ].str();
            std::string args = match[ 2 ].str();
            // printf( "'%s' ( '%s' )\n", func.c_str(), args.c_str() );

            args = std::regex_replace( args, std::regex( "\"" ), "\\\"" );

            if( func.compare( "ERROR" ) == 0 )
            {
                error.push_back( ErrorInfo{ std::to_string( cnt + 1 ), args } );
                continue;
            }
            else
            {
                fprintf( stderr,
                    "%s:%i: error: unknown/invalid 'tc' command found: '%s'\n",
                    file_name, cnt, mstr.c_str() );

                exit( -1 );
            }

            no_cmd_found = false;
            break;
        }
    } );

    std::string fn( file_name );
    std::replace( fn.begin(), fn.end(), '/', '_' );
    std::replace( fn.begin(), fn.end(), '.', '_' );
    std::replace( fn.begin(), fn.end(), '-', '_' );
    std::replace( fn.begin(), fn.end(), ' ', '_' );

    FILE* fd = 0;

    if( mode.compare( "tc" ) == 0 )
    {
        std::string fn_tc( dest_name );
        fn_tc += ".tc.cpp";

        fd = fopen( fn_tc.c_str(), "w+" );
        assert( fd );

        fprintf( fd,
            "\n"
            "#ifndef _LIB_CASMTC_UTS_RUNNER_\n"
            "#define _LIB_CASMTC_UTS_RUNNER_\n"
            "#include \"gtest/gtest.h\"\n"
            "#include \"cpp/Default.h\"\n"
            "#include \"uts/RunnerTest.h\"\n"
            "#endif //_LIB_CASMTC_UTS_RUNNER_\n"
            "\n"
            "INSTANTIATE_TEST_CASE_P\n"
            "( libcasm_tc__%s\n"
            ", RunnerTest\n"
            ", ::testing::Values\n"
            "  ( RunnerTestParam\n"
            "    { \"%s\"\n"
            "    , \"%s\"\n"
            "    , {",
            fn.c_str(), file_name, fn_tc.c_str() );

        u1 first_error = true;
        for( auto& e : error )
        {
            // printf( "::: '%s' '%s'\n", e.line.c_str(), e.code.c_str() );

            fprintf( fd,
                "%s { \"%s\", \"%s\" }\n"
                "      ",
                first_error ? "" : ",", e.line.c_str(), e.code.c_str() );
            first_error = false;
        }

        fprintf( fd,
            "}\n"
            "    }\n"
            "  )\n"
            ");\n"
            "\n" );

        assert( fclose( fd ) == 0 );
    }

    if( mode.compare( "bm" ) == 0 )
    {
        std::string fn_bm( dest_name );
        fn_bm += ".bm.cpp";

        fd = fopen( fn_bm.c_str(), "w+" );
        assert( fd );

        fprintf( fd,
            "\n"
            "#ifndef _LIB_CASMTC_UTS_BENCHMARKS_\n"
            "#define _LIB_CASMTC_UTS_BENCHMARKS_\n"
            "#include \"uts/RunnerBenchmark.h\"\n"
            "#endif //_LIB_CASMTC_UTS_BENCHMARKS_\n"
            "\n"
            "BM\n"
            "( %s\n"
            ", \"%s\"\n"
            ");\n"
            "\n",
            fn.c_str(), file_name );

        assert( fclose( fd ) == 0 );
    }

    return 0;
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
