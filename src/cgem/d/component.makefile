this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(SOURCE_DIR)/src/build/StdComponent.makefile

INSTALL_DIR := $(BUILD_DIR)/install
DGEM_LIB_INSTALL_DIR := $(INSTALL_DIR)/d/lib
DGEM_LIB_PATH := $(DGEM_LIB_INSTALL_DIR)/libdgem.a
DGEM_SO_PATH := $(DGEM_LIB_INSTALL_DIR)/libdgem.so
 
.PHONY: .d_link
.d_link: FIND_CFG := -type f -name '*.d.o' -and -not -name 'test.d.o'
.d_link: $(.d_targets)
	@$(ECHO) $(call color-str,$(BIYel),Linking $@: $^)
	mkdir -p $(DGEM_LIB_INSTALL_DIR)
	find $(.d_this_build) $(FIND_CFG) -exec $(AR) -rs $(DGEM_LIB_PATH) {} +
	$(AR) -t -v $(DGEM_LIB_PATH)

# Add .d_link dependency to .d_build target
.d_build: .d_link

