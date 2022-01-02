//
//  Copyright (C) 2014-2022 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//                <https://github.com/casm-lang/libcasm-tc/graphs/contributors>
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

#include "Profile.h"

#include <libstdhl/String>

#include <cassert>
#include <cstring>

using namespace libcasm_tc;

Profile::Identifier Profile::fromString( const std::string& str )
{
    const auto tmp = libstdhl::String::replaceAll( str, "\n", "" );

    for( std::size_t c = 0; c < Identifier::_SIZE_; c++ )
    {
        const auto id = static_cast< Identifier >( c );
        if( toString( id ) == tmp )
        {
            return id;
        }
    }

    throw std::invalid_argument( "unsupported profile '" + tmp + "'" );
    return Identifier::_SIZE_;
}

std::string Profile::toString( const Identifier id )
{
    switch( id )
    {
        case Identifier::INTERPRETER:
        {
            return "interpreter";
        }
        case Identifier::COMPILER:
        {
            return "compiler";
        }
        case Identifier::FORMAT:
        {
            return "format";
        }
        case Identifier::LANGUAGE_SERVER:
        {
            return "language-server";
        }
        case Identifier::_SIZE_:
        {
            throw std::invalid_argument( "unsupported profile id" );
            return "";
        }
    }
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
