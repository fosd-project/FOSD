ARCH = x86_64
IMAGE = img

KERNEL_COMPILER  =
KERNEL_CCOPTIONS = -m64 -ffreestanding -nostdlib -mcmodel=large \
-mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
-fpack-struct=1 -masm=intel \
-Werror=return-type -fno-stack-protector \
-fno-use-cxa-atexit -fno-threadsafe-statics \
-fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
KERNEL_LINKERSCRIPT    = linker.ld
KERNEL_LDOPTIONS = -m elf_x86_64 -nostdlib

KERNEL_ASOPTIONS = -f elf64

USER_SOURCESPATH  = examples/x86_64/HelloWorld
USER_INCLUDEPATH  =

OUTPUT_FILENAME = OS

# compiler settings

AS = nasm
CC = g++
LD = ld
OBJDUMP = objdump
OBJCOPY = objcopy
