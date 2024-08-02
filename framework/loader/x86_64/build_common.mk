# build_common.mk

# Required arguments : 
# LOADER_CC : gcc(or g++) compiler for the loader
# LOADER_LD : the ld compiler for the loader
# LOADER_AS : the assembler "
# LOADER_CCOPTIONS : gcc(or g++) compiler options
# LOADER_ASOPTIONS : assembler options
# LOADER_LDOPTIONS : ld compiler options
# LOADER_LINKERSCRIPT : path to the linker script
# LOADER_LD_OUT       : output file name for the ld compiler
# C_EXTENSION         : C/C++ source file's extension
# ASM_EXTENSION       : assembly source file's extension

# SOURCESFOLDER	   : the path to the source folder
# INCLUDEPATHS     : includepaths for the C/C++ sources

# LINK_OBJECTS[yes/no] : select whether the compiled C/C++ objects will be linked
#                        If this options is set to "yes," compiled objects are linked using the ld options. 

include $(ROOTDIR)/configurations.mk
include $(ROOTDIR)/global_variables.mk

BINARYFOLDER = $(ROOTDIR)/$(ROOTBINARYFOLDER)/$(LOADERFOLDER)/$(ARCH)/$(IMAGE)

clean:
	rm -rf $(BINARYFOLDER)

AUTO_CC_TARGETS = $(subst .$(C_EXTENSION),.obj,$(wildcard $(SOURCESFOLDER)/*.$(C_EXTENSION)))
AUTO_AS_TARGETS = $(subst .$(AS_EXTENSION),.aobj,$(wildcard $(SOURCESFOLDER)/*.$(AS_EXTENSION)))

ifeq ($(LINK_OBJECTS),yes)
all: prepare build_bootloader $(AUTO_CC_TARGETS) $(AUTO_AS_TARGETS) build_objects build_final_image
else
all: prepare build_bootloader $(AUTO_CC_TARGETS) $(AUTO_AS_TARGETS) build_final_image
endif

prepare:
	mkdir -p $(BINARYFOLDER)

build_objects:
	$(LOADER_LD) $(wildcard $(BINARYFOLDER)/*.obj) $(wildcard $(BINARYFOLDER)/*.aobj) -T $(LOADER_LINKERSCRIPT) -o $(LOADER_LD_OUT) $(LOADER_LDOPTIONS)

%.obj: %.$(C_EXTENSION)
	$(LOADER_CC) -c $< -o $(subst $(SOURCESFOLDER),$(BINARYFOLDER),$@) $(LOADER_CCOPTIONS)\
		$(addprefix  -I ,$(INCLUDEPATHS))

%.aobj: %.$(AS_EXTENSION)
	$(LOADER_AS) $(COMMON_SOURCEFOLDER)/$@ -o $(MAINBINARYFOLDER)/$(subst .asm,.obj,$@)
