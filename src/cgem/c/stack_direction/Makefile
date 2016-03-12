this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))

sources := stack_direction.c
objects := $(patsubst %.c,$(this_build)/%.c.o,$(sources))

$(sp)_targets += $(objects) $(this_build)/test.c.o
$(sp)_targets += $(this_build)/stack_direction.c.pic.o

.PHONY: $(sp)_build
$(sp)_build: $($(sp)_targets)
	@$(ECHO) $(call color-str,$(BIGre),Done $@: $^)

.PHONY: $(sp)_install
$(sp)_install: THIS_DIR := $(this_dir)
$(sp)_install: INSTALL_INCLUDE := $(BUILD_DIR)/install/include/cgem/c/stack_direction
$(sp)_install: $($(sp)_targets)
	mkdir -p $(INSTALL_INCLUDE)
	cp $(THIS_DIR)/stack_direction.h $(INSTALL_INCLUDE)/.

$(sp)_build: $(sp)_install

$(call std_epilog)

