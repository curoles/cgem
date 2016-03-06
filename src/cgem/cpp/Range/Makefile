this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))

.PHONY: $(sp)_build
$(sp)_build: $(targets)
	@echo "end of $@: $^"


$(call std_epilog)

