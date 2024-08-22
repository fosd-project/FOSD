include configurations.mk
include global_variables.mk

BASH = bash
ROOTDIR = $(PWD)

MAKEFILE_ARGUMENTS = ROOTDIR=$(PWD)
KERNEL_TARGET_OBJECTS = $(wildcard $(ROOTBINARYFOLDER)/$(KERNELBINARYFOLDER)/*.*) \
	$(wildcard $(ROOTBINARYFOLDER)/$(KERNELBINARYFOLDER)/*/*.*) # for nested objects
USER_TARGET_OBJECTS   = $(wildcard $(ROOTBINARYFOLDER)/$(USERBINARYFOLDER)/*.*) \
	$(wildcard $(ROOTBINARYFOLDER)/$(USERBINARYFOLDER)/*/*.*)

prepare:
	@echo "$(shell tput bold)Preparing the directory structure..$(shell tput sgr0)"
	mkdir -p $(ROOTBINARYFOLDER)/$(LOADERFOLDER)/$(ARCH)/$(IMAGE)

remove_binaries:
	rm -rf $(ROOTBINARYFOLDER)/*

clean: remove_binaries
	@echo "$(shell tput bold)Cleaning the files..$(shell tput sgr0)"

	for build_dir in $(BUILD_TARGET_FOLDERS); do\
		make -C $$build_dir clean $(MAKEFILE_ARGUMENTS); \
	done
	make -C $(LOADERFOLDER)/$(ARCH)/$(IMAGE) clean $(MAKEFILE_ARGUMENTS) TARGET_DIR=./output/$(ARCH)/$(IMAGE)

all: prepare build_all_directories build_kernel build_loader
build_framework: prepare build_all_directories

build_all_directories:
	@echo "$(shell tput bold)Building the kernel sources...$(shell tput sgr0)"
	for build_dir in $(BUILD_TARGET_FOLDERS); do\
		make -C $$build_dir all $(MAKEFILE_ARGUMENTS); \
	done

# Assume that the user objects are all compiled
build_kernel:
	@echo "$(shell tput bold)Building the kernel image...$(shell tput sgr0)"
	make -f $(ARCHFOLDER)/$(ARCH)/$(BUILD_KERNEL_IMG_MAKEFILE) build_kernel ROOTDIR=$(PWD) TARGET_OBJECTS="$(KERNEL_TARGET_OBJECTS) $(USER_TARGET_OBJECTS)"\
		LIBRARYFOLDER="." KERNEL_ELF=$(ROOTBINARYFOLDER)/$(KERNEL_ELF) KERNEL_IMG=$(ROOTBINARYFOLDER)/$(KERNEL_IMG)

build_loader:
	@echo "$(shell tput bold)Building the final image...$(shell tput sgr0)"
	mkdir $(ROOTDIR)/output/$(ARCH)/$(IMAGE) -p
	make -C $(LOADERFOLDER)/$(ARCH)/$(IMAGE) all $(MAKEFILE_ARGUMENTS) TARGET_DIR=$(ROOTDIR)/output/$(ARCH)/$(IMAGE) KERNEL_IMG=$(ROOTDIR)/$(ROOTBINARYFOLDER)/$(KERNEL_IMG) KERNEL_ELF=$(ROOTDIR)/$(ROOTBINARYFOLDER)/$(KERNEL_ELF)

.PHONY: clean all