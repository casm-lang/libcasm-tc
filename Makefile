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

TARGET = casm-tc

include .config.mk


MK_DIR := $(shell cd $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))); pwd)


$(OBJ)/$(TARGET):
	$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) $(TARGET)

%.casm.tc.cpp: $(OBJ)/$(TARGET) %.casm
	mkdir -p `dirname $(DST)/$(filter %.casm,$^)`
	$(OBJ)/$(TARGET) $(MK_DIR)/$(filter %.casm,$^) $(DST)/$(filter %.casm,$^).tc.cpp


$(OBJ)/$(TARGET)-bm:
	$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) $(TARGET)-bm

%.casm.bm.cpp: $(OBJ)/$(TARGET)-bm %.casm
	mkdir -p `dirname $(DST)/$(filter %.casm,$^)`
	$(OBJ)/$(TARGET)-bm $(MK_DIR)/$(filter %.casm,$^) $(DST)/$(filter %.casm,$^).bm.cpp
