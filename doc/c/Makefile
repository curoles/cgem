this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))
$(if $(debug_make),$(info Current build dir: $($(sp)_this_build)))

.PHONY: $(sp)_build
$(sp)_build: $(targets)
	@echo "end of $@: $^"

$(call std_epilog)
