//  
//  Copyright (c) 2014-2016 CASM Organization
//  All rights reserved.
//  
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/ppaulweber/libcasm-tc
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

#include "stdhl/cpp/Default.h"
#include "stdhl/cpp/File.h"



int main( int argc, const char *argv[] )
{
    assert( argc == 3 );

	const char* file_name = argv[1];
	const char* dest_name = argv[2];
	
	assert( libstdhl::File::exists( file_name ) );
	
	FILE* fd = fopen( dest_name, "w+" );
	assert( fd );
	
	std::string fn( file_name );
	std::replace( fn.begin(), fn.end(), '/', '_');
	std::replace( fn.begin(), fn.end(), '.', '_');
    
	fprintf
	( fd
	, "#include \"gtest/gtest.h\"\n"
	  "TEST( libcasm_tc__%s, )\n"
	  "{\n"
	  "    int ret = 0;\n"
	  "    ASSERT_EQ( ret, 0 );\n"
	, fn.c_str()
	);

	std::vector< std::string > cmd;
	
	libstdhl::File::readLines
	( file_name
	  , [ cmd, fd, file_name ]( u32 cnt, const std::string& line )
	  {
		  std::string c = "";
		  std::regex expr
		  ( "// ((RUN)|(ASSERT_EQ)|(ASSERT_NE))([\\S ]+)"
		  );
		  std::sregex_iterator start( line.begin(), line.end(), expr );
		  std::sregex_iterator end;
		  
		  for( std::sregex_iterator i = start; i != end; i++ )
		  {
			  std::smatch match = *i;
			  std::string run = match.str();
			  run = std::regex_replace( run, std::regex( "%s" ),        std::string( file_name ) );
			  run = std::regex_replace( run, std::regex( "%\\?" ),        "ret" );

			  run = std::regex_replace( run, std::regex( "\\)" ),       ");" );
			  run = std::regex_replace( run, std::regex( "// RUN\\(" ), "ret = system(" );
			  run = std::regex_replace( run, std::regex( "// ASSERT_EQ\\(" ), "ASSERT_EQ(" );
			  run = std::regex_replace( run, std::regex( "// ASSERT_NE\\(" ), "ASSERT_NE(" );
			  
			  run = std::regex_replace( run, std::regex( "%casmi" ),    "../../casmi" );
			  run = std::regex_replace( run, std::regex( "%casmc" ),    "../../casmc" );
			  
			  fprintf
			  ( fd
			  , "    %s\n"
			  , run.c_str()
			  );
			  break;
		  }
	  }
	);
	
	
	fprintf
	( fd
	, "}\n"
	);
    
    return 0;
}



//  
//  Local variables:
//  mode: c++
//  indent-tabs-mode: t
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//  
