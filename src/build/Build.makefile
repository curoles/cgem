#//! @file
#//! @brief This file is ...
#//! @author Igor Lesik

$(info Build.makefile)

#MAKEFILES ?= --output-sync=target

include $(SOURCE_DIR)/src/build/BuildRules.makefile
include $(SOURCE_DIR)/src/build/TerminalAnsiColor.makefile


.PHONY: all
all: all_docs .c_build .cpp_build .d_build

.PHONY: all_docs
all_docs: .doc.index_build .doc.build_build .doc.c_build .doc.cpp_build .doc.d_build

include $(SOURCE_DIR)/doc/component.makefile
include $(SOURCE_DIR)/src/cgem/c/component.makefile
include $(SOURCE_DIR)/src/cgem/cpp/component.makefile
include $(SOURCE_DIR)/src/cgem/d/component.makefile

.PHONY: clean
clean:
	find ./* ! -name Makefile -exec rm -frv {} +
