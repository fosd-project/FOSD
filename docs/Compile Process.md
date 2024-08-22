# The FOSD Framework Specification: The Compile Process
## Overview
The framework consists of two main parts: the **Kernel Image** and the **Loader**. The kernel image is compiled with the **core kernel sources**(located in "kernel" folder), **architecture-dependent sources**(located in "arch" folder), and **user-side sources**(located in the root directory of the project.) All the sources(sources of kernel, architecture and user) are all compiled into object files, and all those objects are linked into one kernel image using the linker script. Finally, the kernel loader is compiled and combined with the compiled kernel image, creating the final operating system image.

The architecture, the compiler and the file type of the final operating system image are all customizable by **configuration makefile(configurations.mk)**, which contains variables that can be customized by user. 

## The Structure of the Project
```
FOSD(Project Root Directory)/
├── framework/
│   ├── arch/
│   │   ├── arch-1/
│   │   │   ├── include
│   │   │   ├── src
│   │   │   ├── build_kernel.mk
│   │   │   └── linker.ld
│   │   ├── arch-2
│   │   └── ...
│   ├── kernel/
│   │   ├── include
│   │   ├── src
│   │   └── Makefile
│   ├── loader/
│   │   ├── arch-1/
│   │   │   ├── file-type-1/
│   │   │   │   ├── include
│   │   │   │   ├── src
│   │   │   │   └── Makefile
│   │   │   ├── file-type-2
│   │   │   └── ...
│   │   ├── arch-2
│   │   ├── ...
│   │   └── build_common.mk
│   ├── configurations.mk
│   ├── framework.mk
│   ├── global_variables.mk
│   └── Makefile
├── [user source files]
└── Makefile
```
The framework consists of big three parts: **arch**, **kernel**, and **loader**. The "arch" folder contains the architecture-dependent parts of the kernel of the framework, and "kernel" folder contains the core parts of the framework kernel. The "loader" folder contains the source for kernel loader, which is in charge of compiling both the kernel loader and the final operating system image. 
### 1. The Main Kernel Folder
The "kernel" folder contains the core parts of the framework's kernel. Using the Makefile on the "kernel" folder, the sources and headers are automatically compiled into the objects, according to the compile options defined on the configuration file. 
### 2. The Architecture Folder
The architecture folder contains the folders for each architecture. Each folder contains the corresponding architecture's architecturally dependent part of the kernel. 

Following is the structure of the architecture folder : 
```
arch/
└── [architecture name]/
    ├── include/
    │   └── header files
    ├── src/
    │   └── source files
    ├── build_kernel.mk
    └── linker.ld (optional)
```
Each architecture folder must contain "build_kernel.mk" makefile that is used for building the final kernel image from the object files. No additional makefile is required, as the makefile system automatically detects the source files on the "src" folder and compiles into the object files. 
### 3. The Loader Folder
The loader folder contains sources of each architecture's loaders. In each architecture folder loaders corresponding to its designated file type are contained on each individual folder. 

Following is the structure of the loader folder : 
```
loader/
├── [architecture XXX]/
│   ├── [file type aaa]/
│   │   ├── include
│   │   ├── src
│   │   └── Makefile
│   └── [file type bbb]/
│       ├── include
│       ├── src
│       └── Makefile
├── [architecture YYY]/
│   ├── [file type aaa]/
│   │   ├── ...
│   │   └── Makefile
│   └── [file type bbb]/
│       ├── ...
│       └── Makefile
└── build_common.mk
```
The compile process of the loader is highly flexible, unlike the compilation of kernel or architecture having an automated compilation of the source files. The compile process of the loader depends mostly on the individual makefile. 
## The Overview of the Makefile Process
The making process of the operating system follows : 
1. **Compile the user-made sources** : The user-made sources in the user directory are compiled into object files. 
2. **Compile the kernel sources** : The source files on the kernel and architecture folder are compiled. 
3. **Link all the objects** : All the objects(user,kernel and architecture) are linked using the (architecture-dependent) linker script. (The process uses "build_kernel.mk" file to link the kernel image)
4. **Compile the loader sources** : The sources of the loader are compiled.
5. **Combine the loader and the kernel image** : The compiled loader and the kernel image are combined into the final operating system image. 

### The Configuration Variables
The makefile build system is designed to be customized by the makefile variables defined in the configuration files. All the configuration variables must be defined in "configurations.mk" file on the framework directory. 

Following variables are designated to be customized by the user : 
 * **ARCH** : Target operating system architecture
 * **IMAGE** : Target operating system image
 * **KERNEL_COMPILER** : Cross compiler for kernel
 * **KERNEL_CCOPTIONS** : Compiler options for g++(or gcc)
 * **KERNEL_LDOPTIONS** : Compiler options for the linker
 * **KERNEL_ASOPTIONS** : Compiler options for the assembler
 * **KERNEL_LINKERSCRIPT** : The linker script file name
 * **USER_SOURCESPATH** : Directory location of user's sources
 * **USER_INCLUDEPATH** : Directory location of user's includepaths

The **"ARCH"** variable points to the architecture folder to be compiled in the "arch" folder. Similarly, the **"IMAGE"** variable points to the loader folder to be compiled in the loader's architecture folder. 

Following is an example of "configurations.mk" file : 
```makefile
ARCH = x86_64
IMAGE = iso

KERNEL_COMPILER  = x86_64-elf
KERNEL_CCOPTIONS = -m64 -ffreestanding -nostdlib -mcmodel=large \
-mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
-fpack-struct=1 -masm=intel ...
KERNEL_LINKERSCRIPT    = linker.ld
KERNEL_LDOPTIONS       = -m elf_x86_64 -nostdlib

# The directory to the user's sources and include from the root project directory
USER_SOURCESPATH = ...
USER_INCLUDEPATH = ...
...
```
### Detailed Compile Process of the Framework
### 1. The Compiling Process of Kernel
#### The "framework.mk" Makefile
The compiling of the operating system can be initiated by the [**"framework.mk"**](https://github.com/fosd-project/FOSD/blob/core/framework/framework.mk) Makefile. The "framework.mk" first compiles the user's source codes and then the full operating system combined with the framework sources.  

"framework.mk" consists of following sections: 
- **clean** : Clean all the object files and target operating system image
- **all**   : Compile the full operating system
- **build_os** : Only compile the framework, not the user's sources
<!-- This is kinda wrong.. -->

"framework.mk" file uses the [Makefile located on the framework root directory](https://github.com/fosd-project/FOSD/blob/core/framework/Makefile) to build the framework's kernel and loader. 
#### The Central Makefile of the Framework
[The central makefile of the framework](https://github.com/fosd-project/FOSD/blob/core/framework/Makefile) reads the configuration variables from the **configurations.mk** and compiles the operating system according to the variables. The makefile sequentially compiles architecture and kernel and builds the kernel image using **the "build_kernel.mk" makefile.** The makefile then initiates the compilation of the loader, synthesizing the final operating system image. 

#### The "build_kernel.mk" Makefile
As the method of building the final kernel image from the C/C++ objects are dependent to the architecture, **build_kernel.mk** is created to account for the difference in the method. Each architecture must contain "build_kernel.mk" that describes the linking method. 

Following is the convention of the "build_kernel.mk" : 
```makefile
include $(ROOTDIR)/configurations.mk
include $(ROOTDIR)/global_variables.mk

build_kernel:
    [commands for building the kernel image from compiled objects]
```
The commands for linking the kernel objects must be implemented on the "build_kernel" section of the makefile. 

Following is the list of several variables passed onto the "build_kernel.mk" makefile : 
- **TARGET_OBJECTS** : Object files that are to be linked
- **KERNEL_ELF** : The target ELF file of the kernel image
- **KERNEL_IMG** : The final target kernel image

"build_kernel.mk" must compile the objects passed onto the pre-declared variable into the final kernel image, also its path passed onto the variable. 

(An example of "build_kernel.mk" implementation on x86_64 architecture : )
```makefile
include $(ROOTDIR)/configurations.mk
include $(ROOTDIR)/global_variables.mk

build_kernel:
	x86_64-elf-ld $(KERNEL_LDOPTIONS)
        -T $(ARCHFOLDER)/$(ARCH)/$(KERNEL_LINKERSCRIPT) $(TARGET_OBJECTS) 
        -o $(KERNEL_ELF)
	x86_64-elf-objcopy -O binary $(KERNEL_ELF) $(KERNEL_IMG)
```
### 2. The Compiling Process of the OS Image(and Loader)
