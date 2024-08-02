FRAMEWORKFOLDER = framework

include $(FRAMEWORKFOLDER)/global_variables.mk
include $(FRAMEWORKFOLDER)/configurations.mk

BINARYFOLDER = $(FRAMEWORKFOLDER)/$(ROOTBINARYFOLDER)/$(USERBINARYFOLDER)

INCLUDEPATHS = . $(ARCHFOLDER)/$(ARCH)/$(COMMON_HEADERFOLDER) $(KERNELFOLDER)/$(COMMON_HEADERFOLDER)

MAINTARGETS = $(subst .cpp,.obj,$(wildcard *.cpp) $(wildcard */*.cpp))
ASMTARGETS = $(subst .asm,.aobj,$(wildcard *.asm))

clean:
	rm -rf $(BINARYFOLDER)
	make -C $(FRAMEWORKFOLDER) clean

all: prepare $(ASMTARGETS) $(MAINTARGETS) complete_build

prepare:
	mkdir -p $(BINARYFOLDER)
	make -C $(FRAMEWORKFOLDER) build_framework

complete_build: # finally build kernel and loader
	make -C $(FRAMEWORKFOLDER) build_kernel build_loader

%.obj: %.cpp
	$(KERNEL_CC) $(KERNEL_CCOPTIONS) -c $< -o $(addprefix $(BINARYFOLDER)/,$@) $(KERNEL_CCOPTIONS)\
		$(addprefix  -I ,$(INCLUDEPATHS))

%.aobj: %.asm
	$(KERNEL_AS) $(COMMON_SOURCEFOLDER)/$@ -o $(BINARYFOLDER)/$(subst .asm,.obj,$@)

.PHONY: clean all