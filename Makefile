BUILD_DIR ?= ../build
INSTALL_DIR ?= ../install

CXX := g++ #/tools/local/gcc-4.7.2/bin/g++-4.7.2
CC  := gcc #/tools/local/gcc-4.7.2/bin/gcc-4.7.2
CXXFLAGS := -std=c++0x #-std=c++11

-include config.mk

#$(realpath $(lastword $(MAKEFILE_LIST)))

BUILD_DIR_PATH = $(abspath $(BUILD_DIR))
INSTALL_DIR_PATH = $(abspath $(INSTALL_DIR))

INSTALL_INCLUDE_DIR := $(INSTALL_DIR_PATH)/include/cgem
INSTALL_LIB_DIR := $(INSTALL_DIR_PATH)/lib

CGEM_LIB := $(INSTALL_LIB_DIR)/libcgem.a
XGEM_LIB := $(INSTALL_LIB_DIR)/libxgem.a

CFLAGS += -I$(INSTALL_DIR_PATH)/include


AR := ar

MAKE_ENV := CC=$(CC) CXX=$(CXX) AR=$(AR)
MAKE_ENV += CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)"
MAKE_ENV += INSTALL_DIR=$(INSTALL_DIR_PATH) BUILD_DIR=$(BUILD_DIR_PATH)
MAKE_ENV += INSTALL_INCLUDE_DIR=$(INSTALL_INCLUDE_DIR) INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)
MAKE_ENV += CGEM_LIB=$(CGEM_LIB) XGEM_LIB=$(XGEM_LIB)

all: $(BUILD_DIR) $(INSTALL_DIR)
	cp helpers/*.h $(INSTALL_INCLUDE_DIR)/.
	make -C Lambda            $(MAKE_ENV)
	make -C mixin             $(MAKE_ENV)
	make -C File              $(MAKE_ENV)
	make -C logging           $(MAKE_ENV)
	make -C StaticString      $(MAKE_ENV)
	make -C StringBuf         $(MAKE_ENV)
	make -C Backtrace         $(MAKE_ENV)
	make -C C/Vector          $(MAKE_ENV)
	make -C C/Trie            $(MAKE_ENV)
	make -C C/stack_direction $(MAKE_ENV)
	make -C JRS               $(MAKE_ENV)
	make -C XML               $(MAKE_ENV)
	$(AR) -t -v $(CGEM_LIB)
	$(AR) -t -v $(XGEM_LIB)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/obj/c
	mkdir -p $(BUILD_DIR)/obj/cpp

$(INSTALL_DIR):
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_INCLUDE_DIR)
	mkdir -p $(INSTALL_INCLUDE_DIR)/C
	mkdir -p $(INSTALL_LIB_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)

.PHONY: clean
