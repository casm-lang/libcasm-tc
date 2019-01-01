//
//  Copyright (C) 2014-2019 CASM Organization <https://casm-lang.org>
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

#include <libcasm-tc/CasmTC>
#include <libstdhl/File>
#include <libstdhl/Log>
#include <libstdhl/Memory>

#include <regex>

using namespace libcasm_tc;

int main( int argc, const char* argv[] )
{
    const auto source =
        libstdhl::Memory::make< libstdhl::Log::Source >( argv[ 0 ], "Test Case Generator Tool" );

    libstdhl::Log::defaultSource( source );

    assert( argc == 5 );

    assert( argv[ 1 ] );
    std::string mode( argv[ 1 ] );

    const char* file_name = argv[ 2 ];
    const char* dest_name = argv[ 3 ];
    const char* path_name = argv[ 4 ];

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
    u1 disabled = false;

    libstdhl::File::readLines(
        file_name,
        [mode, file_name, dest_name, &no_cmd_found, &error, &disabled](
            u32 cnt, const std::string& line ) {
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
                else if( func.compare( "DISABLED" ) == 0 )
                {
                    disabled = true;
                    continue;
                }
                else
                {
                    fprintf(
                        stderr,
                        "%s:%i: error: unknown/invalid 'casm-tc' command found: '%s'\n",
                        file_name,
                        cnt,
                        mstr.c_str() );

                    exit( -1 );
                }

                no_cmd_found = false;
                break;
            }
        } );

    std::string fn( file_name );
    std::string pn( path_name );

    size_t pos = fn.find( pn );
    if( pos != std::string::npos )
    {
        fn.erase( pos, pn.length() );
    }

    std::replace( fn.begin(), fn.end(), '/', '_' );
    std::replace( fn.begin(), fn.end(), '.', '_' );
    std::replace( fn.begin(), fn.end(), '-', '_' );
    std::replace( fn.begin(), fn.end(), ' ', '_' );

    FILE* fd = 0;

    if( mode.compare( "test" ) == 0 )
    {
        fd = fopen( dest_name, "w+" );
        assert( fd );

        fprintf(
            fd,
            "\n"
            "#ifndef _LIBCASM_TC_TESTS_\n"
            "#define _LIBCASM_TC_TESTS_\n"
            "#include <gtest/gtest.h>\n"
            "#include <libcasm-tc/RunnerTest>\n"
            "#endif // _LIBCASM_TC_TESTS_\n"
            "\n"
            "INSTANTIATE_TEST_CASE_P\n"
            "( %slibcasm_tc__%s\n"
            ", RunnerTest\n"
            ", ::testing::Values\n"
            "  ( libcasm_tc::RunnerTestParam\n"
            "    { \"%s\"\n"
            "    , \"%s\"\n"
            "    , {",
            ( disabled ? "DISABLED_" : "" ),
            fn.c_str(),
            file_name,
            dest_name );

        u1 first_error = true;
        for( auto& e : error )
        {
            // printf( "::: '%s' '%s'\n", e.line.c_str(), e.code.c_str() );

            fprintf(
                fd,
                "%s { \"%s\", \"%s\" }\n"
                "      ",
                first_error ? "" : ",",
                e.line.c_str(),
                e.code.c_str() );
            first_error = false;
        }

        fprintf(
            fd,
            "}\n"
            "    }\n"
            "  )\n"
            ");\n"
            "\n" );

        assert( fclose( fd ) == 0 );
    }

    if( mode.compare( "benchmark" ) == 0 )
    {
        fd = fopen( dest_name, "w+" );
        assert( fd );

        fprintf(
            fd,
            "\n"
            "#ifndef _LIBCASM_TC_BENCHMARKS_\n"
            "#define _LIBCASM_TC_BENCHMARKS_\n"
            "#include <libcasm-tc/RunnerBenchmark>\n"
            "#endif // _LIBCASM_TC_BENCHMARKS_\n"
            "\n"
            "INSTANTIATE_BENCHMARK_CASE\n"
            "( %s\n"
            ", %s\n"
            ", \"%s\"\n"
            ");\n"
            "\n",
            ( disabled ? "DISABLED_" : "" ),
            fn.c_str(),
            file_name );

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
