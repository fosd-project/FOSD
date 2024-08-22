BUILD_TARGET_FOLDERS = arch kernel 

COMMON_SOURCEFOLDER = src
COMMON_HEADERFOLDER = include

ARCHFOLDER         = arch
KERNELFOLDER       = kernel
LOADERFOLDER       = loader
ROOTBINARYFOLDER   = bin
KERNELBINARYFOLDER = kernel_objs
USERBINARYFOLDER   = user_objs

BUILD_KERNEL_IMG_MAKEFILE = build_kernel.mk

KERNEL_ELF   = Kernel
KERNEL_IMG   = Kernel.bin

KERNEL_AS = $(AS)
KERNEL_CC = $(KERNEL_COMPILER)-$(CC)
KERNEL_LD = $(KERNEL_COMPILER)-$(LD)
KERNEL_OBJDUMP = $(KERNEL_COMPILER)-$(OBJDUMP)
KERNEL_OBJCOPY = $(KERNEL_COMPILER)-$(OBJCOPY)

# commonly used things

PWD = $(shell pwd)
ARCH_CONFIGURATION_FILE_LOC     = $(ARCHFOLDER)/$(ARCH)
ARCH_CONFIGURATION_FILE         = arch_configurations.hpp

define convert_hpp_to_ld
	echo \#include \"$(notdir $(2))\" > $(dir $(2))dummy.ld
	$(KERNEL_CC) -E -P -xc -DLINKER_SCRIPT $(dir $(2))dummy.ld > $(subst .hpp,.ld,$(2))
	rm $(dir $(2))dummy.ld
endef