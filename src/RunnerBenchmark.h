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

#ifndef _LIB_CASMTC_RUNNER_BENCHMARK_H_
#define _LIB_CASMTC_RUNNER_BENCHMARK_H_

#include "cpp/Default.h"
#include "cpp/File.h"
#include "cpp/Type.h"
#include "hayai.hpp"
#include "libcasm-tc.h"

class bm : public ::hayai::Fixture
{
  private:
    char m_cmd[ 4096 ];

  public:
    virtual void SetUp();
    void run( const char* spec );
    virtual void TearDown();
};

#define BM( FN, SPEC )                                                         \
    BENCHMARK_F( bm, libcasm_tc__##FN, 5, 5 )                                  \
    {                                                                          \
        run( SPEC );                                                           \
    }

#endif // _LIB_CASMTC_RUNNER_BENCHMARK_H_
