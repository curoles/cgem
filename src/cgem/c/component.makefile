this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


include $(SOURCE_DIR)/src/build/StdComponent.makefile


INSTALL_DIR := $(BUILD_DIR)/install
CGEM_LIB_INSTALL_DIR := $(INSTALL_DIR)/c/lib
CGEM_LIB_PATH := $(CGEM_LIB_INSTALL_DIR)/libcgem.a
CGEM_SO_PATH := $(CGEM_LIB_INSTALL_DIR)/libcgem.so
 
.PHONY: .c_link
.c_link: FIND_CFG := -type f -name '*.c.o' -and -not -name 'test.c.o'
.c_link: $(.c_targets)
	@$(ECHO) $(call color-str,$(BIYel),Linking $@: $^)
	mkdir -p $(CGEM_LIB_INSTALL_DIR)
	find $(.c_this_build) $(FIND_CFG) -exec $(AR) -rs $(CGEM_LIB_PATH) {} +
	$(AR) -t -v $(CGEM_LIB_PATH)
 
.PHONY: .c_shared_link
.c_shared_link: FIND_CFG := -type f -name '*.c.pic.o' -and -not -name 'test.c.pic.o'
.c_shared_link: $(.c_targets)
	@$(ECHO) $(call color-str,$(BIYel),Linking $@: $^)
	mkdir -p $(CGEM_LIB_INSTALL_DIR)
	find $(.c_this_build) $(FIND_CFG) -exec $(LD) -shared -o $(CGEM_SO_PATH) {} +
	readelf --symbols $(CGEM_SO_PATH)

# Add .c_link dependency to .c_build target
.c_build: .c_link .c_shared_link


.PHONY: .c_test
.c_test: FIND_TEST_O := find $(.c_this_build) -type f -name 'test.c.o'
.c_test: COMPILE_TEST := $(CC) {} $(CGEM_LIB_PATH) -o test.out
.c_test: FIND_TEST_EXE := find $(.c_this_build) -type f -name 'test.out'
.c_test: $(.c_link)
	@$(ECHO) $(call color-str,$(BIYel),Testing $@: $^)
	$(FIND_TEST_O) -execdir $(COMPILE_TEST) \;
	$(FIND_TEST_EXE) -exec echo -e '$(BIBlu)'Run {} '$(RCol)' \; -execdir $(VALGRIND) {} \;

# Add .c_test dependency to .c_build target
.c_build: .c_test

