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


TARGET += obj/tc

FILES   = $(shell find src -name '*.cpp' | cut -d'.' -f1)
OBJECTS = $(FILES:%=obj/%.o)

INCLUDE  = -I ../
INCLUDE += -I ./


default: $(TARGET)

obj/src/%.o: src/%.cpp
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

$(TARGET): $(OBJECTS)
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) $(INCLUDE) -o $@ $(filter %.o,$^) -lstdc++ -lm

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " test
	@rm -f test


TEST_FILES_CASM  = $(shell grep -lnr uts -e "//@ TC" | sed -e "s/.casm//g")
TEST_FILES_CPP   = $(shell find uts -name "*.cpp" | sed -e "s/.cpp//g")

TEST_CASES       = $(TEST_FILES_CASM:%=obj/%.casm.cpp)

TEST_OBJECTS    += obj/uts_Runner.o
TEST_OBJECTS    += $(TEST_FILES_CPP:%=obj/%.o)

TEST_INCLUDE  = -I ../gtest/googletest/include
TEST_INCLUDE += -I ../gtest/googletest

TEST_LIBRARY  = -lstdc++
TEST_LIBRARY += -lm
TEST_LIBRARY += -lpthread

obj/uts/%.o:   uts/%.cpp
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(TEST_INCLUDE) $(INCLUDE) -c $< -o $@

obj/uts_Runner.cpp: $(TEST_CASES)
	@echo "CAT " $@
	@cat $^ > $@

obj/uts_Runner.o: obj/uts_Runner.cpp
	@mkdir -p `dirname $@`
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(TEST_INCLUDE) $(INCLUDE) -c $< -o $@

obj/uts/%.casm.cpp: $(TARGET) uts/%.casm
	@mkdir -p `dirname $@`
	@echo "TC  " $(filter %.casm,$^)
	@$(TARGET) $(filter %.casm,$^) $@

obj/uts/%.casm: $(TARGET) uts/%.casm
	@mkdir -p `dirname $@`
	@echo "TC  " $(filter %.casm,$^)
	@$(TARGET) $(filter %.casm,$^) $@
	@touch $@

test-cases:
	@( for i in $(TEST_FILES_CASM); do echo $$i; done )

test: $(TEST_OBJECTS)
	@rm -f $@
	@echo "LD  " $@
	@$(CPP) $(CPPFLAG) $(TEST_INCLUDE) $(INCLUDE) $(TEST_LIBRARY) -o $@ $(filter %.o,$^) ../gtest/googletest/src/gtest-all.cc ../gtest/googletest/src/gtest_main.cc

test-run: test
	@echo "RUN " $<
	@./$<
