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

#ifndef _LIB_CASMTC_RUNNER_TEST_H_
#define _LIB_CASMTC_RUNNER_TEST_H_

#include "cpp/Default.h"
#include "cpp/File.h"
#include "cpp/Type.h"

#include "libcasm-tc.h"

#include "gtest/gtest.h"

#define TEST_NAME                                                              \
    ( std::string(::testing::UnitTest::GetInstance()                           \
                      ->current_test_info()                                    \
                      ->test_case_name() )                                     \
        + "." + std::string(::testing::UnitTest::GetInstance()                 \
                                ->current_test_info()                          \
                                ->name() ) )                                   \
        .c_str()

namespace libcasm_tc
{
    struct ParamError
    {
        std::string line;
        std::string code;
    };

    class RunnerTestParam
    {
      public:
        const char* specification;
        const char* output_path;
        std::vector< ParamError > error;
    };

    inline void PrintTo( const RunnerTestParam& obj, ::std::ostream* os ){};
}

class RunnerTest
    : public ::testing::TestWithParam< libcasm_tc::RunnerTestParam >
{
};

#endif // _LIB_CASMTC_RUNNER_TEST_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
