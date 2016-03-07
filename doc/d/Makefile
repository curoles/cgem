this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))
$(if $(debug_make),$(info Current build dir: $($(sp)_this_build)))

$(sp)_targets += $(sp)_doxygen

.PHONY: $(sp)_build
$(sp)_build: $($(sp)_targets)
	@$(ECHO) $(call color-str,$(BIGre),Done $@: $^)

.PHONY: $(sp)_doxygen
$(sp)_doxygen: $($(sp)_this_build)/doxy/Doxyfile
	@$(ECHO) Building D Library Documentation with Doxygen
	$(DOXYGEN) $<

$($(sp)_this_build)/doxy/Doxyfile: OUTPUT_DIR := $($(sp)_this_build)/doxy
$($(sp)_this_build)/doxy/Doxyfile: DOXY_CFG := $(this_dir)/Doxyfile
$($(sp)_this_build)/doxy/Doxyfile: $($(sp)_this_build)/doxy $(DOXY_CFG)
	cp $(DOXY_CFG) $@
	echo "OUTPUT_DIRECTORY=$(OUTPUT_DIR)" >> $@
	echo "INPUT=$(SOURCE_DIR)/doc/d" >> $@
	echo "INPUT+=$(SOURCE_DIR)/src/cgem/d" >> $@
	echo "STRIP_FROM_PATH=$(SOURCE_DIR)" >> $@ 

$($(sp)_this_build)/doxy:
	mkdir -p $@

$(call std_epilog)
