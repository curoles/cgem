$(info Include Build Environment Configuration BuildEnv.makefile)

HOSTNAME := $(shell hostname)
$(info Host: $(HOSTNAME))

CC  := gcc
CXX := g++

DOXYGEN := doxygen
MARKDOWN := markdown
NATURALDOCS := NaturalDocs

CFLAGS := -I$(SOURCE_DIR)/src

ifeq ($(HOSTNAME),pc104.smi.local)

CC  := /tools/local/gcc-5.2.0/bin/gcc
CXX := /tools/local/gcc-5.2.0/bin/g++
MARKDOWN := /local_disk/igor/tools/markdown/markdown
NATURALDOCS := /local_disk/igor/tools/NaturalDocs/NaturalDocs1.51/NaturalDocs
include $(SOURCE_DIR)/utm/Mentor.Env.makefile

else ifeq ($(HOSTNAME),pc29.smi.local)
else ifeq ($(HOSTNAME),macL)
#MAKE = /home/igor/tools/make/make-4.1/install/bin
VLIB := echo
VLOG := echo
else
#
endif

$(info CC : $(CC))
$(info CXX: $(CXX))
