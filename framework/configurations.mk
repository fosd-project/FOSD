ARCH = x86_64
IMAGE = debugimgtype

KERNEL_COMPILER  = x86_64-elf
KERNEL_CCOPTIONS = -m64 -ffreestanding -nostdlib -mcmodel=large \
-mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
-fpack-struct=1 -masm=intel \
-Werror=return-type -fno-stack-protector \
-fno-use-cxa-atexit -fno-threadsafe-statics \
-fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
KERNEL_LINKERSCRIPT    = linker.ld
KERNEL_LDOPTIONS = -m elf_x86_64 -nostdlib

KERNEL_ASOPTIONS = -f elf64

# compiler settings

AS = nasm
CC = g++
LD = ld
OBJDUMP = objdump
OBJCOPY = objcopy