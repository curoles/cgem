this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(SOURCE_DIR)/src/build/std.makefile

$(call std_prolog)

.PHONY: $(sp)_build
$(sp)_build:

$(call std_epilog)

# PHYS_DIR := $(DIG_DIR)/physics
# 
# PHYS_SRC := $(PHYS_DIR)/realnum.d
# PHYS_SRC += $(PHYS_DIR)/quantity.d
# PHYS_SRC += $(PHYS_DIR)/temperature.d
# PHYS_SRC += $(PHYS_DIR)/length.d
# PHYS_SRC += $(PHYS_DIR)/time.d
# PHYS_SRC += $(PHYS_DIR)/velocity.d
# 
# PHYS_OPTS := $(PHYS_SRC)
# PHYS_OPTS += -w
# PHYS_OPTS += -lib
# #PHYS_OPTS += -H -Hd$(BLD_DIR) -Hf$(BLD_DIR)/physics.di
# PHYS_OPTS += -unittest
# PHYS_OPTS += -od$(BLD_DIR) -oflibphysics -op $(DMD_GENDOC)/physics
# 
# physics:
# 	$(DC) $(PHYS_OPTS)
# 
# 
# 
# .PHONY: physics
