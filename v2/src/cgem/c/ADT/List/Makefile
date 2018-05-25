this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))

$(sp)_targets += $(this_build)/test.c.o

.PHONY: $(sp)_build
$(sp)_build: $($(sp)_targets)
	@$(ECHO) $(call color-str,$(BIGre),Done $@: $^)

.PHONY: $(sp)_install
$(sp)_install: THIS_DIR := $(this_dir)
$(sp)_install: INSTALL_INCLUDE := $(BUILD_DIR)/install/include/cgem/c/ADT/List
$(sp)_install: $($(sp)_targets)
	mkdir -p $(INSTALL_INCLUDE)
	cp $(THIS_DIR)/List.h $(INSTALL_INCLUDE)/.

$(sp)_build: $(sp)_install

$(call std_epilog)

