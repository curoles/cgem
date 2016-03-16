this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)

$(sp)_targets += $(this_build)/test.cpp.o

.PHONY: $(sp)_build
$(sp)_build: $($(sp)_targets)
	@$(ECHO) $(call color-str,$(BIGre),Done $@: $^)


$(call std_epilog)

