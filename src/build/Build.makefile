#//! @file
#//! @brief This file is ...
#//! @author Igor Lesik

$(info Build.makefile)

#MAKEFILES ?= --output-sync=target

include $(SOURCE_DIR)/src/build/BuildRules.makefile


.PHONY: all
all: .doc_build

#.src_cgem_c

#.src_c .src_cpp .doc_c .doc_cpp .doc_design

include $(SOURCE_DIR)/doc/component.makefile
#include $(SOURCE_DIR)/src/cgem/c/component.makefile

.PHONY: clean
clean:
	find ./* ! -name Makefile -exec rm -frv {} +
