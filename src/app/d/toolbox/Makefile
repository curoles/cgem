this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)
$(if $(debug_make),$(info Processing $(sp).Makefile))
$(if $(debug_make),$(info Current build dir: $($(sp)_this_build)))


sources := main.d wc.d tc.d zip.d
sources := $(patsubst %.d,$(this_dir)/%.d,$(sources))

#objects := $(patsubst %.c,$(this_build)/%.c.o,$(sources))

# $(sp)_targets += 

.PHONY: $(sp)_build
$(sp)_build: APP_EXE_PATH := $($(sp)_this_build)/toolbox
$(sp)_build: $($(sp)_targets)
	mkdir -p $(dir $(APP_EXE_PATH))
	$(DC) $(sources) -I$(SOURCE_DIR)/src/ -o $(APP_EXE_PATH)
	@$(ECHO) $(call color-str,$(BIGre),Done $@: $^)


$(call std_epilog)

# TBOX_DIR := $(DIG_DIR)/toolbox
# 
# TBOX := $(TBOX_DIR)/main.d
# TBOX += $(TBOX_DIR)/wc.d
# TBOX += $(TBOX_DIR)/tc.d
# TBOX += $(TBOX_DIR)/zip.d
# 
# TBOX += $(DIG_DIR)/io/file.d
# TBOX += $(DIG_DIR)/idiom/block.d
# 
# TBOX_OPTIONS := $(BLD_DIR)/libphysics.a $(TBOX)
# TBOX_OPTIONS += -I$(SRC_DIR)/source
# TBOX_OPTIONS += -unittest
# TBOX_OPTIONS += -w
# TBOX_OPTIONS += -od$(BLD_DIR) -of$(INSTALL_DIR)/toolbox -op $(DMD_GENDOC)/toolbox
# 
# toolbox: $(TBOX) physics
# 	@echo Compiling with $(DC)
# 	$(DC) $(TBOX_OPTIONS)
# 
# 
# .PHONY: toolbox
