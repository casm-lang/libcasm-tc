#   
#   Copyright (c) 2014-2016 CASM Organization
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 Emmanuel Pescosta
#                 https://github.com/casm-lang/libcasm-tc
#   
#   This file is part of libcasm-tc.
#   
#   libcasm-tc is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   libcasm-tc is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with libcasm-tc. If not, see <http://www.gnu.org/licenses/>.
#   

AR=ar

CC=clang
CCFLAG += -std=c11
CCFLAG += -g -O0
CCFLAG += -Wall

CPP=clang
CPPFLAG += -std=c++11
CPPFLAG += -g -O0
CPPFLAG += -Wall
#CPPFLAG += -Wextra


TARGET_TC += obj/tc
TARGET_BM += obj/bm


FILES_TC   = src/tc #$(shell find src -name '*.cpp' | cut -d'.' -f1)
OBJECTS_TC = $(FILES_TC:%=obj/%.o)

FILES_BM   = src/bm #$(shell find src -name '*.cpp' | cut -d'.' -f1)
OBJECTS_BM = $(FILES_BM:%=obj/%.o)

INCLUDE  = -I ../
INCLUDE += -I ./


default: $(TARGET_TC) $(TARGET_BM)

obj/src/%.o: src/%.cpp
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

$(TARGET_TC): $(OBJECTS_TC)
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) $(INCLUDE) -o $@ $(filter %.o,$^) -lstdc++ -lm

$(TARGET_BM): $(OBJECTS_BM)
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) $(INCLUDE) -o $@ $(filter %.o,$^) -lstdc++ -lm

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " test
	@rm -f test
	@echo "RM  " benchmark
	@rm -f benchmark


TEST_FILES_CASM  = $(shell grep -lnr uts -e "//@ TC" | sed -e "s/.casm//g")
TEST_FILES_CPP   = uts/RunnerTest #$(shell find uts -name "*.cpp" | sed -e "s/.cpp//g")

TEST_CASES       = $(TEST_FILES_CASM:%=obj/%.tc.cpp)

TEST_OBJECTS    += obj/uts_Tests.o
TEST_OBJECTS    += $(TEST_FILES_CPP:%=obj/%.o)

TEST_INCLUDE  = -I ../gtest/googletest/include
TEST_INCLUDE += -I ../gtest/googletest

TEST_LIBRARY  = -lstdc++
TEST_LIBRARY += -lm
TEST_LIBRARY += -lpthread

obj/uts/RunnerTest.o: uts/RunnerTest.cpp uts/RunnerTest.h
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(TEST_INCLUDE) $(INCLUDE) -c $< -o $@

obj/uts_Tests.cpp: $(TEST_CASES)
	@echo "CAT " $@
	@cat $^ > $@

obj/uts_Tests.o: obj/uts_Tests.cpp
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(TEST_INCLUDE) $(INCLUDE) -c $< -o $@

obj/uts/%.tc.cpp: $(TARGET_TC) uts/%.casm
	@mkdir -p `dirname $@`
	@echo "TC  " $(filter %.casm,$^)
	@$(TARGET_TC) $(filter %.casm,$^) $@

tests:
	@( for i in $(TEST_FILES_CASM); do echo $$i; done )

test: $(TEST_OBJECTS)
	@rm -f $@
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) $(TEST_INCLUDE) $(INCLUDE) $(TEST_LIBRARY) -o $@ $(filter %.o,$^) ../gtest/googletest/src/gtest-all.cc ../gtest/googletest/src/gtest_main.cc

test-run: test
	@echo "RUN " $<
	@./$<



BENCHMARK_FILES_CASM  = $(shell grep -lnr uts -e "//@ BM" | sed -e "s/.casm//g")
BENCHMARK_FILES_CPP   = uts/RunnerBenchmark #$(shell find uts -name "*.cpp" | sed -e "s/.cpp//g")

BENCHMARK_CASES       = $(BENCHMARK_FILES_CASM:%=obj/%.bm.cpp)

BENCHMARK_OBJECTS    += obj/uts_Benchmarks.o
BENCHMARK_OBJECTS    += $(BENCHMARK_FILES_CPP:%=obj/%.o)

BENCHMARK_INCLUDE  = -I ../hayai/src

BENCHMARK_LIBRARY  = -lstdc++
# BENCHMARK_LIBRARY += -lm
# BENCHMARK_LIBRARY += -lpthread

obj/uts/RunnerBenchmark.o: uts/RunnerBenchmark.cpp uts/RunnerBenchmark.h
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(BENCHMARK_INCLUDE) $(INCLUDE) -c $< -o $@

obj/uts_Benchmarks.cpp: $(BENCHMARK_CASES)
	@echo "CAT " $@
	@cat $^ > $@

obj/uts_Benchmarks.o: obj/uts_Benchmarks.cpp
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(BENCHMARK_INCLUDE) $(INCLUDE) -c $< -o $@

obj/uts/%.bm.cpp: $(TARGET_BM) uts/%.casm
	@mkdir -p `dirname $@`
	@echo "BM  " $(filter %.casm,$^)
	@$(TARGET_BM) $(filter %.casm,$^) $@

benchmarks:
	@( for i in $(BENCHMARK_FILES_CASM); do echo $$i; done )

benchmark: $(BENCHMARK_OBJECTS)
	@rm -f $@
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) $(BENCHMARK_INCLUDE) $(INCLUDE) $(BENCHMARK_LIBRARY) -o $@ $(filter %.o,$^) ../hayai/src/hayai_posix_main.cpp

benchmark-run: benchmark
	@echo "RUN " $<
	@./$<

