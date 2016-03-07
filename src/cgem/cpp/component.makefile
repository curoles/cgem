this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(SOURCE_DIR)/src/build/StdComponent.makefile

INSTALL_DIR := $(BUILD_DIR)/install
CPPGEM_LIB_INSTALL_DIR := $(INSTALL_DIR)/cpp/lib
CPPGEM_LIB_PATH := $(CPPGEM_LIB_INSTALL_DIR)/libcppgem.a
 
.PHONY: .cpp_link
.cpp_link: FIND_CFG := -type f -name '*.o' -and -not -name 'test.cpp.o'
.cpp_link: $(.cpp_targets)
	@echo $(call color-str,$(BIYel),Linking $@: $^)
	mkdir -p $(CPPGEM_LIB_INSTALL_DIR)
	find $(.cpp_this_build) $(FIND_CFG) -exec $(AR) -rcs $(CPPGEM_LIB_PATH) {} +
	find -path $(CPPGEM_LIB_PATH) -exec $(AR) -t -v $(CPPGEM_LIB_PATH) \;
 

# Add .cpp_link dependency to .cpp_build target
.cpp_build: .cpp_link


.PHONY: .cpp_test
.cpp_test: FIND_TEST_O := find $(.cpp_this_build) -type f -name 'test.cpp.o'
.cpp_test: COMPILE_TEST := $(CXX) {} $(CPPGEM_LIB_PATH__FIXME) -o test.out
.cpp_test: FIND_TEST_EXE := find $(.cpp_this_build) -type f -name 'test.out'
.cpp_test: $(.cpp_link)
	@echo $(call color-str,$(BIYel),Testing $@: $^)
	$(FIND_TEST_O) -execdir $(COMPILE_TEST) \;
	$(FIND_TEST_EXE) -exec echo Running {} \; -execdir {} \;

# Add .cpp_test dependency to .cpp_build target
.cpp_build: .cpp_test

