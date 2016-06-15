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

	for( i32 s = fn.size()-1; s >= 0; s-- )
	{
		if( fn.c_str()[s] == '/' )
		{
			fn.replace( s, 1, ", ");
			break;			
		}
	}
	
	std::replace( fn.begin(), fn.end(), '/', '_');
	std::replace( fn.begin(), fn.end(), '.', '_');
	
	fprintf
	( fd
	, "#include \"gtest/gtest.h\"\n"
	  "TEST( libcasm_tc__%s )\n"
	  "{\n"
	  "    int ret = 0;\n"
	  "    ASSERT_EQ( ret, 0 );\n"
	, fn.c_str()
	);

	std::vector< std::string > cmd;
	u1 no_tc_command_found = true;
	
	libstdhl::File::readLines
	( file_name
	, [ fd, file_name, dest_name, &no_tc_command_found ]
	  ( u32 cnt, const std::string& line )
	  {
		  std::string c = "";
		  std::regex expr
		  ( "//@ ([\\S]+)([\\(]([\\S ])+[\\)])"
		  );
		  std::sregex_iterator start( line.begin(), line.end(), expr );
		  std::sregex_iterator end;
		  
		  for( std::sregex_iterator i = start; i != end; i++ )
		  {
			  std::smatch match = *i;
			  assert( match.size() == 4 );
			  
			  std::string mstr   = match.str();
			  // printf( "'%s'\n", mstr.c_str() );			  
			  // for( int c = 0; c < match.size(); c++ )
			  // {
			  // 	  printf( "%i:'%s'\n", c, match[c].str().c_str() );
			  // }
			  
			  std::string func = match[1].str();
			  std::string args = match[2].str();

			  args = std::regex_replace( args, std::regex( "\"" ),       "\\\"" );
			  
			  if( func.compare( "RUN" ) == 0 )
			  {
				  func = "ret = system";
				  args = std::regex_replace( args, std::regex( "\\(" ),       "(\"" );
				  args = std::regex_replace( args, std::regex( "\\)" ),       "\")" );
			  }
			  else if
			  (   not func.compare( "ASSERT_EQ" )
			  and not func.compare( "ASSERT_NE" )
			  )
			  {
				  fprintf
				  ( stderr
				  , "%s:%i: error: unknown/invalid 'tc' command found: '%s'\n"
				  , file_name
				  , cnt
				  , mstr.c_str()
				  );
				  
				  exit(-1);
			  }

			  std::string fstdout = std::string( dest_name ) + ".stdout";
			  std::string fstderr = std::string( dest_name ) + ".stderr";
			  
			  args = std::regex_replace( args, std::regex( "\\)" ),       ");" );
			  
			  args = std::regex_replace( args, std::regex( "%s" ),        std::string( file_name ) );
			  args = std::regex_replace( args, std::regex( "%o" ),        fstdout );
			  args = std::regex_replace( args, std::regex( "%e" ),        fstderr );
			  args = std::regex_replace( args, std::regex( "%\\?" ),      "ret" );
			  
			  args = std::regex_replace( args, std::regex( "%casmi" ),    "../../casmi" );
			  args = std::regex_replace( args, std::regex( "%casmc" ),    "../../casmc" );
			  
			  fprintf
			  ( fd
			  , "    %s%s\n"
			  , func.c_str()
			  , args.c_str()
			  );

			  no_tc_command_found = false;
			  break;
		  }
	  }
	);
	
	if( no_tc_command_found )
	{
		fprintf
	    ( fd
		, "    EXPECT_FALSE( \"'%s' does not use any 'tc' commands\" );\n"
		, file_name
		);
	}
	
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
