//
//  Copyright (c) 2014-2017 CASM Organization
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

#ifndef _LIB_CASMTC_PROFILE_H_
#define _LIB_CASMTC_PROFILE_H_

namespace libcasm_tc
{
    /**
       @extends CasmTC

     */
    class Profile
    {
      public:
        enum Identifier
        {
            INTERPRETER = 0,
            COMPILER,
            FORMAT,
            LANGUAGE_SERVER,
            IDENTIFIER_MAX
        };

        static const char* get( const char* id )
        {
            static const char* uid[] = { "interpreter", "compiler", "format" };

            if( (i64)id == -1 )
            {
                return (const char*)0;
            }
            else if( (i64)id >= 0 and ( i64 ) id < IDENTIFIER_MAX )
            {
                return uid[ (i64)id ];
            }
            else
            {
                for( i64 i = 0; i < IDENTIFIER_MAX; i++ )
                {
                    if( strcmp( uid[ i ], id ) == 0 )
                    {
                        return (const char*)i;
                    }
                }
            }
            return 0;
        }

        static const char* get( Identifier id )
        {
            return get( (const char*)id );
        }
    };
}

#endif // _LIB_CASMTC_PROFILE_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
