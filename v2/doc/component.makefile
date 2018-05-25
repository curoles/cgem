this_dir := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(SOURCE_DIR)/src/build/StdComponent.makefile

