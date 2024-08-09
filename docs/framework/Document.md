# The FOSD Framework
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

### Detailed Compile Process of the Kernel
#### The "build_kernel.mk" Makefile
As the method of building the final kernel image from the C/C++ objects are dependent to the architecture, **build_kernel.mk** is created to account for the difference in the method. Each architecture must contain "build_kernel.mk" that describes the linking method. 

Following is the convention of the "build_kernel.mk" : 
```makefile
include $(ROOTDIR)/configurations.mk
include $(ROOTDIR)/global_variables.mk

build_kernel:
    [commands for building the kernel image from compiled objects]
```


### Detailed Compile Process of the Architecture

##