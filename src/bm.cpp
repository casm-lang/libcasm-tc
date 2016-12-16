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
    assert( argc == 3 );

    const char* file_name = argv[ 1 ];
    const char* dest_name = argv[ 2 ];

    if( not libstdhl::File::exists( file_name ) )
    {
        libstdhl::Log::error( "unable to open file '%s'", file_name );
        assert( 0 );
    }

    std::vector< std::string > cmd;
    u1 no_bm_found = true;
    u1 no_bm_command_found = true;

    struct ErrorInfo
    {
        std::string line;
        std::string code;
    };

    std::vector< ErrorInfo > error;

    libstdhl::File::readLines( file_name, [file_name, dest_name, &no_bm_found,
                                              &no_bm_command_found,
                                              &error]( u32 cnt,
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

            if( func.compare( "BM" ) == 0 )
            {
                no_bm_found = false;
            }
            else if( func.compare( "TC" ) == 0 or func.compare( "ERROR" ) == 0 )
            {
                continue;
            }
            else
            {
                fprintf( stderr,
                    "%s:%i: error: unknown/invalid 'bm' command found: '%s'\n",
                    file_name, cnt, mstr.c_str() );

                exit( -1 );
            }

            no_bm_command_found = false;
            break;
        }
    } );

    std::string fn( file_name );
    std::replace( fn.begin(), fn.end(), '/', '_' );
    std::replace( fn.begin(), fn.end(), '.', '_' );
    std::replace( fn.begin(), fn.end(), '-', '_' );

    FILE* fd = 0;
    fd = fopen( dest_name, "w+" );
    assert( fd );

    if( no_bm_found or no_bm_command_found )
    {
        fprintf( fd,
            "// no BM command found\n"
            "\n" );

        assert( fclose( fd ) == 0 );
        return 0;
    }

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
