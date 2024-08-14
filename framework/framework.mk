FRAMEWORKFOLDER = framework

include $(FRAMEWORKFOLDER)/global_variables.mk
include $(FRAMEWORKFOLDER)/configurations.mk

BINARYFOLDER = $(FRAMEWORKFOLDER)/$(ROOTBINARYFOLDER)/$(USERBINARYFOLDER)

INCLUDEPATHS = . $(FRAMEWORKFOLDER)/$(ARCHFOLDER)/$(ARCH)/$(COMMON_HEADERFOLDER) \
			   $(FRAMEWORKFOLDER)/$(KERNELFOLDER)/$(COMMON_HEADERFOLDER) \
			   $(FRAMEWORKFOLDER)/$(ARCH_CONFIGURATION_FILE_LOC) \
			   $(USER_INCLUDEPATH) \

# Compile targets for user sources
MAINTARGETS = $(subst .cpp,.obj,$(wildcard $(USER_SOURCESPATH)/*.cpp) $(wildcard $(USER_SOURCESPATH)/*/*.cpp))
ASMTARGETS = $(subst .asm,.aobj,$(wildcard $(USER_SOURCESPATH)/*.asm))

clean:
	rm -rf $(BINARYFOLDER)
	make -C $(FRAMEWORKFOLDER) clean

all: prepare build_user build_os

prepare:
	mkdir -p $(BINARYFOLDER)
	make -C $(FRAMEWORKFOLDER) build_framework

build_user: $(ASMTARGETS) $(MAINTARGETS) 

build_os: # finally build kernel and loader
	make -C $(FRAMEWORKFOLDER) build_kernel build_loader

%.obj: %.cpp
	$(KERNEL_CC) $(KERNEL_CCOPTIONS) -c $< -o $(subst $(USER_SOURCESPATH),$(BINARYFOLDER),$@) $(KERNEL_CCOPTIONS)\
		$(addprefix  -I,$(INCLUDEPATHS))

%.aobj: %.asm
	$(KERNEL_AS) $< -o $(subst $(USER_SOURCESFOLDER),$(BINARYFOLDER),$@) $(KERNEL_ASOPTIONS)

.PHONY: clean all