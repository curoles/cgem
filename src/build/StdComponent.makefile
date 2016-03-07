# Caller must initialize var this_dir
# this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
# before including this file.
#
# See Debugging Makefiles, http://www.oreilly.com/openbook/make3/book/ch12.pdf

$(if $(debug_make),$(info Include StdComponent.makefile from $(this_dir)))

include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)

$(if $(debug_make),$(info Current Build Stack Pointer is $(sp)))
$(if $(debug_make),$(info Current targets: $($(sp)_targets)))

.PHONY: $(sp)_build
$(sp)_build: $($(sp)_targets)
	@$(ECHO) $(call color-str,$(BIGre),Done $@)

$(call std_epilog)

