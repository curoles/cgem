this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


include $(SOURCE_DIR)/src/build/StdComponent.makefile

.PHONY: .c_link
.c_link: $(.c_targets)
	@echo $(call color-str,$(BIYel),Linking $@: $^)

# Add .c_link prereq dependency to .c_build target
.c_build: .c_link

