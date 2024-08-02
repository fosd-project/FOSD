# Required arguments : 
# TARGET_DIR    : Location to the directory of the argument
# KERNEL_ELF    : Location to the kernel elf file
# KERNEL_IMG    : Location to the kernel binary file

LOADER_CC = x86_64-linux-gnu-gcc
LOADER_LD = x86_64-linux-gnu-ld
LOADER_AS = nasm
LOADER_OBJCOPY = x86_64-linux-gnu-objcopy

LOADER_CCOPTIONS = -m32 -Os -masm=intel -fno-builtin -fpack-struct=1 -fno-stack-protector \
	-Wno-implicit-function-declaration \
	-Wno-int-to-pointer-cast \
	-W -Wall
LOADER_ASOPTIONS = -f elf32
LOADER_LDOPTIONS = -m elf_i386
LOADER_LINKERSCRIPT = linker.ld
LOADER_LD_OUT = $(BINARYFOLDER)/loader32.elf

C_EXTENSION = c
AS_EXTENSION = asm

SOURCESFOLDER = src
INCLUDEPATHS = include

LINK_OBJECTS = yes

build_bootloader:
	$(LOADER_AS) bootloader.asm -f bin -o $(BINARYFOLDER)/bootloader.bin

build_final_image:
	$(LOADER_OBJCOPY) -O binary $(LOADER_LD_OUT) $(BINARYFOLDER)/loader32.bin

	./imgmaker $(TARGET_DIR)/OS.img $(BINARYFOLDER)/bootloader.bin $(BINARYFOLDER)/loader32.bin $(KERNEL_IMG)

include ../build_common.mk
	