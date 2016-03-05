define std_prolog
$(eval this_name := $(notdir $(this_dir:%/=%)));
$(eval sp := $(sp).$(this_name));
$(eval $(sp)_this_dir := $(this_dir));
$(eval dirs := $(sort $(dir $(wildcard $(this_dir)/*/Makefile))));
$(eval $(sp)_mfiles := $(foreach dir,$(dirs),$(dir)/Makefile));
$(eval targets := $(foreach dir,$(dirs),$(sp).$(notdir $(dir:%/=%))_build));
$(eval $(sp)_this_rel := $(this_dir:$(SOURCE_DIR)%=%));
$(eval this_build := $(BUILD_DIR)/$($(sp)_this_rel));
$(eval $(sp)_this_build := $(this_build));
$(if $(wildcard $(this_dir)/README.md),$(eval targets += $(this_build)README.md.html));
endef

define std_epilog
$(foreach mfile,$($(sp)_mfiles),$(eval include $(mfile)));
$(eval sp := $(basename $(sp)));
endef

define sv_target 
$(1:%=$(this_build)%.vsim)
endef

