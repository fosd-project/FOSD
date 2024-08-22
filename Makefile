include ./global_variables.mk
include ./configurations.mk

BINARYFOLDER = ./$(ROOTBINARYFOLDER)/$(USERBINARYFOLDER)

INCLUDEPATHS = . ./$(ARCHFOLDER)/$(ARCH)/$(COMMON_HEADERFOLDER) \
			   ./$(KERNELFOLDER)/$(COMMON_HEADERFOLDER) \
			   ./$(ARCH_CONFIGURATION_FILE_LOC) \
			   $(USER_INCLUDEPATH) \

# Compile targets for user sources
MAINTARGETS = $(subst .cpp,.obj,$(wildcard $(USER_SOURCESPATH)/*.cpp) $(wildcard $(USER_SOURCESPATH)/*/*.cpp))
ASMTARGETS = $(subst .asm,.aobj,$(wildcard $(USER_SOURCESPATH)/*.asm))

all: prepare build_user build_os

clean:
	rm -rf $(BINARYFOLDER)
	make -f framework.mk clean

prepare:
	mkdir -p $(BINARYFOLDER)
	make -f framework.mk build_framework
	

build_user: $(ASMTARGETS) $(MAINTARGETS) 

build_os: # finally build kernel and loader
	make -f framework.mk build_kernel build_loader

%.obj: %.cpp
	$(KERNEL_CC) $(KERNEL_CCOPTIONS) -c $< -o $(subst $(USER_SOURCESPATH),$(BINARYFOLDER),$@) $(KERNEL_CCOPTIONS)\
		$(addprefix  -I,$(INCLUDEPATHS))

%.aobj: %.asm
	$(KERNEL_AS) $< -o $(subst $(USER_SOURCESFOLDER),$(BINARYFOLDER),$@) $(KERNEL_ASOPTIONS)

run:
	make -C $(LOADERFOLDER)/$(ARCH)/$(IMAGE) run_os ROOTDIR=$(PWD)

.PHONY: clean all