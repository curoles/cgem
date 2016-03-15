this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)


sources := file.d
objects := $(patsubst %.d,$(this_build)/%.d.o,$(sources))

$(sp)_targets += $(objects)

.PHONY: $(sp)_build
$(sp)_build: $($(sp)_targets)
	@$(ECHO) $(call color-str,$(BIGre),Done $@: $^)

$(call std_epilog)

