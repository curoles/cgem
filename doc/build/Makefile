# Documentation makefile.
#

this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))

targets += $(sp)_doxygen

.PHONY: $(sp)_build
$(sp)_build: $(targets)
	@echo "end of $@: $^"

.PHONY: $(sp)_doxygen
$(sp)_doxygen: $($(sp)_this_build)/doxy/Doxyfile
	@echo Building Build Documentation with Doxygen
	$(DOXYGEN) $<

$($(sp)_this_build)/doxy/Doxyfile: OUTPUT_DIR := $($(sp)_this_build)/doxy
$($(sp)_this_build)/doxy/Doxyfile: $($(sp)_this_build)/doxy $(this_dir)/Doxyfile
	cp $(this_dir)/Doxyfile $@
	echo "OUTPUT_DIRECTORY=$(OUTPUT_DIR)" >> $@
	echo "INPUT=$(SOURCE_DIR)/MakeBuild.makefile $(SOURCE_DIR)/src/build" >> $@

$($(sp)_this_build)/doxy:
	mkdir -p $@

#( cat Doxyfile ; echo "PROJECT_NUMBER=1.0" ) | doxygen -

#.PHONY: $(sp)_natural_docs
#$(sp)_natural_docs: INPUT := --source $(SOURCE_DIR)/dv
#$(sp)_natural_docs: OUTPUT_DIR := $($(sp)_this_build)/nd
#$(sp)_natural_docs: OUTPUT := --output HTML $(OUTPUT_DIR)
#$(sp)_natural_docs: PROJECT := --project $(OUTPUT_DIR)
#$(sp)_natural_docs: $($(sp)_this_build)/nd
#	$(NATURALDOCS) $(INPUT) $(OUTPUT) $(PROJECT)
#
#$($(sp)_this_build)/nd: OUTPUT_DIR := $($(sp)_this_build)/nd
#$($(sp)_this_build)/nd: ND_CFG_DIR := $(this_dir)/naturaldocs
#$($(sp)_this_build)/nd: $(this_dir)/naturaldocs/Languages.txt $(this_dir)/naturaldocs/Topics.txt
#	mkdir -p $@
#	cp $(ND_CFG_DIR)/Languages.txt $(OUTPUT_DIR)/.
#	cp $(ND_CFG_DIR)/Topics.txt $(OUTPUT_DIR)/.

$(call std_epilog)
