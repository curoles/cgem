BUILD_DIR ?= ../build
INSTALL_DIR ?= ../install

#$(realpath $(lastword $(MAKEFILE_LIST)))

BUILD_DIR_PATH = $(abspath $(BUILD_DIR))
INSTALL_DIR_PATH = $(abspath $(INSTALL_DIR))

INSTALL_INCLUDE_DIR := $(INSTALL_DIR_PATH)/include/cgem

CXX := /tools/local/gcc-4.7.2/bin/g++-4.7.2
CFLAGS := -I$(INSTALL_DIR_PATH)/include

MAKE_ENV := CXX=$(CXX) CFLAGS=$(CFLAGS)
MAKE_ENV += INSTALL_DIR=$(INSTALL_DIR_PATH) BUILD_DIR=$(BUILD_DIR_PATH)
MAKE_ENV += INSTALL_INCLUDE_DIR=$(INSTALL_INCLUDE_DIR)

all: $(BUILD_DIR) $(INSTALL_DIR)
	make -C Lambda $(MAKE_ENV)
	make -C mixin $(MAKE_ENV)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(INSTALL_DIR):
	mkdir -p $(INSTALL_DIR)
	mkdir -p $(INSTALL_INCLUDE_DIR)

clean:
	@echo $(MAKEFILE_LIST)
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)

.PHONY: clean
