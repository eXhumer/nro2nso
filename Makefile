SOURCE	:=	source
INCLUDE	:=	include
BIN		:=	bin
LIBS	:=	libs

VERSION_MAJOR	:=	1
VERSION_MINOR	:=	0
VERSION_PATCH	:=	0
VERSION	:=	$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
APP_AUTHOR	:=	$(shell whoami)

DEFFLAGS	:=	-DVERSION_MAJOR=$(VERSION_MAJOR)\
				-DVERSION_MINOR=$(VERSION_MINOR)\
				-DVERSION_PATCH=$(VERSION_PATCH)\
				-DAPP_AUTHOR=$(APP_AUTHOR)

OUTPUT	:=	$(BIN)/nro2nsp

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