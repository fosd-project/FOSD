# ROOTDIR variable is passed by the makefile

include $(ROOTDIR)/configurations.mk
include $(ROOTDIR)/global_variables.mk

BINARYFOLDER = $(ROOTDIR)/$(ROOTBINARYFOLDER)/$(KERNELBINARYFOLDER)

MAINTARGETS = $(subst .cpp,.obj,$(wildcard $(ARCH)/$(COMMON_SOURCEFOLDER)/*.cpp) $(wildcard $(ARCH)/$(COMMON_SOURCEFOLDER)/*/*.cpp))
ASMTARGETS = $(subst .asm,.aobj,$(wildcard $(ARCH)/$(COMMON_SOURCEFOLDER)/*.asm))
SUBDIRECTORIES = $(subst $(COMMON_SOURCEFOLDER),$(BINARYFOLDER),$(sort $(dir $(wildcard $(COMMON_SOURCEFOLDER)/*/))))

INCLUDEPATHS = $(ARCH)/$(COMMON_HEADERFOLDER)

all: prepare $(ASMTARGETS) $(MAINTARGETS)

prepare:
	mkdir -p $(BINARYFOLDER)
ifneq "$(SUBDIRECTORIES)" ""
	mkdir -p $(SUBDIRECTORIES)
endif
	
clean:
	rm -rf $(BINARYFOLDER)

%.obj: %.cpp
	$(KERNEL_CC) $(KERNEL_CCOPTIONS) -c $< -o $(subst $(ARCH)/$(COMMON_SOURCEFOLDER),$(BINARYFOLDER),$@) $(KERNEL_CCOPTIONS)\
		$(addprefix  -I ,$(INCLUDEPATHS))

%.aobj: %.asm
	$(KERNEL_AS) $(COMMON_SOURCEFOLDER)/$@ -o $(BINARYFOLDER)/$(subst .asm,.obj,$@) $(KERNEL_ASOPTIONS)

.PHONY: clean all
