// Global architecture-specific configurations for both kernel and loader.

#ifndef _ARCH_CONFIGURATIONS_HPP_
#define _ARCH_CONFIGURATIONS_HPP_

#define KERNEL_LOAD_LOCATION 0x200000

#ifdef LINKER_SCRIPT

_KERNEL_LOAD_LOCATION = KERNEL_LOAD_LOCATION;

#endif

#endif