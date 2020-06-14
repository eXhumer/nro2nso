# nro2nso - Convert Nintendo Switch Homebrew Application to NSO
# Copyright (C) 2020 eXhumer
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program located in the file LICENSE. If not, see
# <https://www.gnu.org/licenses/>.
 
SOURCE	:=	source
INCLUDE	:=	include
BIN		:=	bin
LIBS	:=	libs

APP_NAME	:=	nro2nso

VERSION_MAJOR	:=	1
VERSION_MINOR	:=	0
VERSION_PATCH	:=	0
VERSION	:=	$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
APP_AUTHOR	:=	$(shell whoami)

DEFFLAGS	:=	-DVERSION_MAJOR=$(VERSION_MAJOR)\
				-DVERSION_MINOR=$(VERSION_MINOR)\
				-DVERSION_PATCH=$(VERSION_PATCH)\
				-DAPP_AUTHOR=$(APP_AUTHOR)\
				-DAPP_NAME=\"$(APP_NAME)\"

OUTPUT	:=	$(BIN)/$(APP_NAME)

LIBLZ4	:=	$(LIBS)/lz4

LIBCXXOPTS	:=	$(LIBS)/cxxopts

LIBDIRS	:=	$(LIBLZ4) $(LIBCXXOPTS)

LIBFLAGS	:=	-llz4

INCLUDEFLAGS	:=	$(foreach dir,$(LIBDIRS),-I$(dir)/include) -I$(INCLUDE)

LIBDIRFLAGS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

SOURCEFILES	:=	$(wildcard $(SOURCE)/*.cpp)\
				$(wildcard $(SOURCE)/*.c)

CXXFLAGS	:=	-Wall -std=gnu++2a $(LIBFLAGS) $(DEFFLAGS)\
				$(INCLUDEFLAGS) $(LIBDIRFLAGS)

BUILD_FILES	:=	$(OUTPUT)

BUILD_FOLDERS	:=	$(BIN)

CXX	:=	g++

CXXOPTS_LICENSE	:=	$(LIBCXXOPTS)/LICENSE

LZ4_LICENSE	:=	$(LIBLZ4)/LICENSE

LIB_LICENSE_CHECK	:=	$(CXXOPTS_LICENSE) $(LZ4_LICENSE)

.PHONY: $(BUILD_FOLDERS) release libs clean cleanlibs

release: $(OUTPUT)
$(OUTPUT): libs $(BUILD_FOLDERS)
libs: $(LIB_LICENSE_CHECK)

clean:
	rm -f $(BUILD_FILES)

cleanlibs:
	$(MAKE) --directory=$(LIBLZ4) --no-print-directory clean

libs:
	$(MAKE) --directory=$(LIBLZ4) --no-print-directory

$(BUILD_FOLDERS):
	[ -d $@ ] || mkdir -p $@

$(OUTPUT):
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCEFILES)

$(CXXOPTS_LICENSE):
	$(error License missing for cxxopts submodule. Make sure to initialize project git submodules before trying to build the project!)

$(LZ4_LICENSE):
	$(error License missing for lz4 submodule. Make sure to initialize project git submodules before trying to build the project!)