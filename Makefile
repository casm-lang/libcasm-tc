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

.PHONY:

default: debug

help:
	@echo "TODO"

REPO = libcasm-tc

TARGET_TC += obj/tc
TARGET_BM += obj/bm

TARGET  = $(TARGET_TC)
TARGET += $(TARGET_BM)

UT = test-$(REPO)-ut
BT = test-$(REPO)-bm

FILES_TC   = src/tc #$(shell find src -name '*.cpp' | cut -d'.' -f1)
OBJECTS_TC = $(FILES_TC:%=obj/%.o)

FILES_BM   = src/bm #$(shell find src -name '*.cpp' | cut -d'.' -f1)
OBJECTS_BM = $(FILES_BM:%=obj/%.o)


CP  = $(shell find src -name '*.cpp' | cut -d'.' -f1)
CO  = $(CP:%=obj/%.o)

CI += -I ./
CI += -I ../

CL  =

CC  =
CF  =

  %-gcc: CC = gcc
%-clang: CC = clang

  debug-%: CF += -O0 -g
release-%: CF += -O3 -DNDEBUG

linux%:  CF += -Wall -std=c++11
linux%:  XF += -Wall -std=c11
linux3%: CF += -m32
linux6%: CF += -m64


build: config $(TARGET)
check: build $(UT)
bench: build $(TARGET_BM) $(BT)

linux32-build: build
linux64-build: build

linux32-check: check
linux64-check: check

linux32-bench: bench
linux64-bench: bench

  debug-build-linux32-gcc:   linux32-build
  debug-check-linux32-gcc:   linux32-check
  debug-bench-linux32-gcc:   linux32-bench
release-build-linux32-gcc:   linux32-build
release-check-linux32-gcc:   linux32-check
release-bench-linux32-gcc:   linux32-bench

  debug-build-linux64-gcc:   linux64-build
  debug-check-linux64-gcc:   linux64-check
  debug-bench-linux64-gcc:   linux64-bench
release-build-linux64-gcc:   linux64-build
release-check-linux64-gcc:   linux64-check
release-bench-linux64-gcc:   linux64-bench

  debug-build-linux32-clang: linux32-build
  debug-check-linux32-clang: linux32-check
  debug-bench-linux32-clang: linux32-bench
release-build-linux32-clang: linux32-build
release-check-linux32-clang: linux32-check
release-bench-linux32-clang: linux32-bench

  debug-build-linux64-clang: linux64-build
  debug-check-linux64-clang: linux64-check
  debug-bench-linux64-clang: linux64-bench
release-build-linux64-clang: linux64-build
release-check-linux64-clang: linux64-check
release-bench-linux64-clang: linux64-bench


  debug:   debug-build-linux64-clang
release: clean release-build-linux64-clang

test:           debug-check-linux64-clang
test-release: release-check-linux64-clang

benchmark:           debug-bench-linux64-clang
benchmark-release: release-bench-linux64-clang


config: CFG=CC="$(CC)" CF="$(CF)"
config:
	@echo "CFG  $(CFG)"


obj/%.o: %.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(CI) -c $< -o $@

obj/%.o: %.c
	@mkdir -p `dirname $@`
	@echo "C   " $<
	@$(CC) $(CF) $(CI) -c $< -o $@

$(TARGET_TC): $(OBJECTS_TC)
	@mkdir -p `dirname $@`
	@echo "LD  " $@
	@$(CC) $(CF) $(CI) -o $@ $(filter %.o,$^) -lstdc++ -lm

$(TARGET_BM): $(OBJECTS_BM)
	@mkdir -p `dirname $@`
	@echo "LD  " $@
	@$(CC) $(CF) $(CI) -o $@ $(filter %.o,$^) -lstdc++ -lm

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " $(UT)
	@rm -f $(UT)
	@echo "RM  " $(BT)
	@rm -f $(BT)


#UT = $(TARGET:%.a=%-test.a)

TEST_FILES_CASM  = $(shell grep -lnr uts -e "//@ TC" | sed -e "s/.casm//g")
TEST_FILES_CPP   = uts/RunnerTest #$(shell find uts -name "*.cpp" | sed -e "s/.cpp//g")

TEST_CASES       = $(TEST_FILES_CASM:%=obj/%.tc.cpp)

TO    += obj/uts_Tests.o
TO    += $(TEST_FILES_CPP:%=obj/%.o)

TI  = -I ../gtest/googletest/include
TI += -I ../gtest/googletest

TL  = -lstdc++
TL += -lm
TL += -lpthread


obj/uts/RunnerTest.o: uts/RunnerTest.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(TI) $(CI) -c $< -o $@

obj/uts_Tests.cpp: $(TEST_CASES)
	@echo "CAT " $@
	@cat $^ > $@

obj/uts_Tests.o: obj/uts_Tests.cpp uts/RunnerTest.h
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(TI) $(CI) -c $< -o $@

obj/uts/%.tc.cpp: $(TARGET_TC) uts/%.casm
	@mkdir -p `dirname $@`
	@echo "TC  " $(filter %.casm,$^)
	@$(TARGET_TC) $(filter %.casm,$^) $@

tests:
	@( for i in $(TEST_FILES_CASM); do echo $$i; done ) | less

# test: $(TO)
# 	@rm -f $@
# 	@echo "LD  " $@
# 	@$(CC) $(CF) $(TI) $(CI) $(TL) -o $@ $(filter %.o,$^) ../gtest/googletest/src/gtest-all.cc ../gtest/googletest/src/gtest_main.cc


$(UT): $(TO) $(CO) $(TARGET)
	@echo "LD  " $@
	@$(CC) \
	  $(CF) \
	  $(TI) \
	  $(CI) \
	  $(TL) \
	  -o $@ \
	  $(TO) \
	  ../gtest/googletest/src/gtest-all.cc \
	  ../gtest/googletest/src/gtest_main.cc 



test-run: test
	@echo "RUN " $(UT)
	@./$(UT)



BENCHMARK_FILES_CASM  = $(shell grep -lnr uts -e "//@ BM" | sed -e "s/.casm//g")
BENCHMARK_FILES_CPP   = uts/RunnerBenchmark #$(shell find uts -name "*.cpp" | sed -e "s/.cpp//g")

BENCHMARK_CASES       = $(BENCHMARK_FILES_CASM:%=obj/%.bm.cpp)

BO    += obj/uts_Benchmarks.o
BO    += $(BENCHMARK_FILES_CPP:%=obj/%.o)

BI  = -I ../hayai/src

BL  = -lstdc++
# BL += -lm
# BL += -lpthread

obj/uts/RunnerBenchmark.o: uts/RunnerBenchmark.cpp
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(BI) $(CI) -c $< -o $@

obj/uts_Benchmarks.cpp: $(BENCHMARK_CASES)
	@echo "CAT " $@
	@cat $^ > $@

obj/uts_Benchmarks.o: obj/uts_Benchmarks.cpp uts/RunnerBenchmark.h
	@mkdir -p `dirname $@`
	@echo "C++ " $<
	@$(CC) $(CF) $(BI) $(CI) -c $< -o $@

obj/uts/%.bm.cpp: $(TARGET_BM) uts/%.casm
	@mkdir -p `dirname $@`
	@echo "BM  " $(filter %.casm,$^)
	@$(TARGET_BM) $(filter %.casm,$^) $@

benchmarks:
	@( for i in $(BENCHMARK_FILES_CASM); do echo $$i; done )

# benchmark: $(BO)
# 	@rm -f $@
# 	@echo "LD  " $@
# 	@$(CC) $(CF) $(BI) $(CI) $(BL) -o $@ $(filter %.o,$^) ../hayai/src/hayai_posix_main.cpp

$(BT): $(BO) $(CO) $(TARGET)
	@echo "LD  " $@
	@$(CC) \
	  $(CF) \
	  $(BI) \
	  $(CI) \
	  $(BL) \
	  -o $@ \
	  $(BO) \
	  ../hayai/src/hayai_posix_main.cpp


benchmark-run: benchmark
	@echo "RUN " $(BT)
	@./$(BT)
