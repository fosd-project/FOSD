# Required arguments : 
# TARGET_DIR    : Location to the directory of the argument
# KERNEL_ELF    : Location to the kernel elf file
# KERNEL_IMG    : Location to the kernel binary file

include $(ROOTDIR)/configurations.mk

LOADER_CC = gcc
LOADER_LD = ld
LOADER_AS = nasm
LOADER_OBJCOPY = objcopy

LOADER_CCOPTIONS = -m32 -Os -masm=intel -fno-builtin -fpack-struct=1 -fno-stack-protector \
	-Wno-implicit-function-declaration \
	-Wno-int-to-pointer-cast \
	-W -Wall
LOADER_ASOPTIONS = -f elf32
LOADER_LDOPTIONS = -m elf_i386 -z noexecstack
LOADER_LINKERSCRIPT = linker.ld
LOADER_LD_OUT = $(BINARYFOLDER)/loader32.elf

C_EXTENSION = c
AS_EXTENSION = asm

SOURCESFOLDER = src
INCLUDEPATHS = include

LINK_OBJECTS = yes

TARGET_DIR=$(ROOTDIR)/output/$(ARCH)/$(IMAGE)
TARGET = $(OUTPUT_FILENAME).img

remove_target:
	rm -rf $(TARGET_DIR)/$(TARGET)

build_bootloader:
	$(LOADER_AS) bootloader.asm -f bin -o $(BINARYFOLDER)/bootloader.bin

build_final_image:
	$(LOADER_OBJCOPY) -O binary $(LOADER_LD_OUT) $(BINARYFOLDER)/loader32.bin

	./imgmaker $(TARGET_DIR)/$(TARGET) $(BINARYFOLDER)/bootloader.bin $(BINARYFOLDER)/loader32.bin 8 $(KERNEL_IMG) 48

run_os: $(TARGET_DIR)/$(TARGET)
	qemu-system-x86_64 -drive format=raw,file=$(TARGET_DIR)/$(TARGET)

include ../../build_common.mk
	