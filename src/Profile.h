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

#ifndef _LIB_CASMTC_PROFILE_H_
#define _LIB_CASMTC_PROFILE_H_

#include <libcasm-tc/CasmTC>

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
            static const char* uid[]
                = { "interpreter", "compiler", "format", "language-server" };

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
