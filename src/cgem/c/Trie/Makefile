this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))

$(sp)_targets += $(this_build)/trie.c.o $(this_build)/test.c.o
$(sp)_targets += $(this_build)/trie.c.pic.o

.PHONY: $(sp)_build
$(sp)_build: $($(sp)_targets)
	@$(ECHO) $(call color-str,$(BIGre),Done $@: $^)


$(call std_epilog)

