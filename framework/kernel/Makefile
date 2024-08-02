# ROOTDIR is passed by the argument
ifndef ROOTDIR
	echo "rootdir not specified!"
endif

include $(ROOTDIR)/configurations.mk
include $(ROOTDIR)/global_variables.mk

BINARYFOLDER = $(ROOTDIR)/$(ROOTBINARYFOLDER)
MAINBINARYFOLDER = $(BINARYFOLDER)/$(KERNELBINARYFOLDER)

MAINTARGETS = $(subst .cpp,.obj,$(wildcard $(COMMON_SOURCEFOLDER)/*.cpp) $(wildcard $(COMMON_SOURCEFOLDER)/*/*.cpp))
ASMTARGETS = $(subst .asm,.aobj,$(wildcard $(COMMON_SOURCEFOLDER)/*.asm))
SUBDIRECTORIES = $(subst $(COMMON_SOURCEFOLDER),$(MAINBINARYFOLDER),$(sort $(dir $(wildcard $(COMMON_SOURCEFOLDER)/*/))))
INCLUDEPATHS = $(ROOTDIR)/$(ARCHFOLDER)/$(ARCH)/$(COMMON_HEADERFOLDER) \
			   $(COMMON_HEADERFOLDER)

all: prepare $(ASMSOURCES) $(MAINTARGETS)

prepare:
	mkdir -p $(MAINBINARYFOLDER)
ifneq "$(SUBDIRECTORIES)" ""
	mkdir -p $(SUBDIRECTORIES)
endif

clean:
	rm -rf $(MAINBINARYFOLDER)

%.obj: %.cpp
	$(KERNEL_CC) -c $< -o $(subst $(COMMON_SOURCEFOLDER),$(MAINBINARYFOLDER),$@) $(KERNEL_CCOPTIONS)\
		$(addprefix  -I ,$(INCLUDEPATHS))

%.aobj: %.asm
	$(KERNEL_AS) $(COMMON_SOURCEFOLDER)/$@ -o $(MAINBINARYFOLDER)/$(subst .asm,.obj,$@) $(KERNEL_ASOPTIONS)

.PHONY: clean all 