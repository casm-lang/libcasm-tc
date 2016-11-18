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

#include "uts/RunnerTest.h"


TEST_P( RunnerTest, case )
{
    Param const& param = GetParam();

    
    EXPECT_NE( (u64)param.specification, 0 );
    EXPECT_TRUE( libstdhl::File::exists( param.specification ) );
    
    u32 exec_result = 0;
    ASSERT_EQ( exec_result, 0 );
    
    std::unordered_map< std::string, const char* > env;
    env[ "EXPORT" ] = "export";
    env[ "ECHO" ] = "echo";
    env[ "CAT" ] = "cat";
    env[ "CASM" ];
    env[ "CASM_TC" ];
    env[ "CASM_ARG_PRE" ];
    env[ "CASM_ARG_POST" ];
    
    for( auto& e : env )
    {
        if( e.second == 0 )
        {
            const char* env_data = getenv( e.first.c_str() );
            if( not env_data )
            {
                env_data = "";
            }
            env[ e.first ] = env_data;
        }
        
        // printf( "%s='%s'\n", e.first.c_str(), e.second );
    }
    
    char cmd[4096];
    
    ASSERT_STRNE( env[ "CASM" ], "" );
    
    std::string tc   = std::string( param.output_path ) + ".tc";
    std::string fout = std::string( param.output_path ) + ".stdout";
    std::string ferr = std::string( param.output_path ) + ".stderr";
    
    sprintf( cmd, "%s -t > %s", env[ "CASM" ], tc.c_str() );
    // printf( "exec: '%s'\n", cmd );
    exec_result = system( cmd );
    ASSERT_EQ( exec_result, 0 );
    
    FILE* TC = fopen( tc.c_str(), "r" );
    fgets( cmd, 4096, TC );
    setenv( "CASM_TC", cmd, 1 );
    
    env[ "CASM_TC" ] = getenv( "CASM_TC" );
    EXPECT_NE( (u64)env[ "CASM_TC" ], 0 );
    EXPECT_STRNE( env[ "CASM_TC" ], "" );
    
    const char* uid = libcasm_tc::Profile::get( env[ "CASM_TC" ] );
    
    switch( (u64)uid )
    {
        case libcasm_tc::Profile::INTERPRETER:
        {
            sprintf
            ( cmd
            , "%s %s %s %s > %s 2> %s"
            , env[ "CASM" ]
            , env[ "CASM_ARG_PRE" ]
            , param.specification
            , env[ "CASM_ARG_POST" ]
            , fout.c_str()
            , ferr.c_str()
            );            
            break;
        }
        default:
        {
            FAIL();
        }
    }
    
    exec_result = system( cmd );
    
    sprintf
    ( cmd
    , "%s %s; %s %s"
    , env[ "CAT" ]
    , fout.c_str()
    , env[ "CAT" ]
    , ferr.c_str()
    );
    
    u64 error_cnt = 0;
    u64 warning_cnt = 0;
    
    libstdhl::File::readLines
    ( ferr.c_str()
    , [ &error_cnt, &warning_cnt ]
      ( u32 cnt, const std::string& line )
      {
          std::regex e( "error:" );
          std::sregex_iterator start( line.begin(), line.end(), e );
          std::sregex_iterator end;
          
          for( std::sregex_iterator i = start; i != end; i++ )
          {
              std::smatch match = *i;
              std::string mstr   = match.str();

	      std::regex expr( "@([\\S]+)\\{([\\S]+)\\}" );
	      std::sregex_iterator expr_start( line.begin(), line.end(), expr );
              std::sregex_iterator expr_end;
              
              for( std::sregex_iterator i = expr_start; i != expr_end; i++ )
              {
                  std::smatch match = *i;
                  assert( match.size() == 3 );
                  std::string mstr   = match.str();
                  //printf( "'%s'\n", mstr.c_str() );              
		  error_cnt++;
	      }
          }
	  
          std::regex w( "warning:" );
          start = std::sregex_iterator( line.begin(), line.end(), w );
          std::sregex_iterator we;
	  
          for( std::sregex_iterator i = start; i != we; i++ )
          {
              std::smatch match = *i;
              std::string mstr   = match.str();
              //printf( "'%s'\n", mstr.c_str() );              
              warning_cnt++;
          }
      }
    );
    
    if( param.error.size() == 0 )
    {
        EXPECT_EQ( exec_result, 0 );
        if( exec_result != 0 )
        {
            system( cmd );
        }
        else if( error_cnt or warning_cnt )
        {
            sprintf
            ( cmd
            , "%s %s"
            , env[ "CAT" ]
            , ferr.c_str()
            );
            
            system( cmd );
        }
    }
    else
    {
        EXPECT_NE( exec_result, 0 );

        u64 failure_cnt = 0;
        std::vector< ParamError > checked;
        
	libstdhl::File::readLines
        ( ferr.c_str()
        , [ &param, &checked, &failure_cnt ]
          ( u32 cnt, const std::string& line )
	  {
	      std::string c = "";
	      std::regex expr
              ( "@([\\S]+)\\{([\\S]+)\\}"
              );
	      
              std::sregex_iterator start( line.begin(), line.end(), expr );
              std::sregex_iterator end;
              
              for( std::sregex_iterator i = start; i != end; i++ )
              {
                  std::smatch match = *i;
                  assert( match.size() == 3 );
                  std::string mstr   = match.str();
                  
                  u1 line_not_found = true;
                  u1 code_not_valid = true;
                  
                  for( auto& e : param.error )
                  {
                      if( e.line.compare( match[1].str() ) == 0 ) // found line!
                      {
		          line_not_found = false;
                          
                          EXPECT_STREQ( e.code.c_str(), match[2].str().c_str() );
                          if( e.code.compare( match[2].str() ) == 0 )
                          {
                              code_not_valid = false;
                              checked.push_back( e );
                          }
                      }
                  }
                  
                  EXPECT_FALSE( line_not_found );
                  EXPECT_FALSE( code_not_valid );
	      
                  if( line_not_found or code_not_valid )
                  {
                      failure_cnt++;
                  }
              }
	  }
	);
        
        EXPECT_EQ( param.error.size(), checked.size() );
        EXPECT_EQ( param.error.size(), error_cnt );
    
        if( exec_result == 0
        or  param.error.size() != checked.size()
    or  param.error.size() != error_cnt
        or  failure_cnt > 0
        )
        {
        printf( "%lu, %lu\n", param.error.size(), error_cnt );
        
            system( cmd );
        }
    }
}


// INSTANTIATE_TEST_CASE_P
// ( libcasm_tc__uts_RunnerTest
// , RunnerTest
// , ::testing::Values
//   ( Param
//     { "text.txt"
//     }
//   , Param
//     { "text.txt"
//     }
//   )
// );

