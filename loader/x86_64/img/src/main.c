#include <intel_paging.h>
#include <arch_configurations.hpp>
#include <basicstring.h>

// The purpose of the kernel loader is copying the kernel to the proper location and switch the system mode to 64-bit. 
// The kernel loader first copies the kernel image, constructs the page table for 64-bit mode, and finally switches into 64-bit long mode. 

#define PAGE_TABLE_LOCATION 0x100000
#define BYTES_PER_SECTOR    512

void clear_screen(int color);
void print_debug_str(int x , int y , int color , const char *str);
void debug_printf(int x , int y , int color , const char *fmt , ...);
void jump_to_longmode(unsigned int kernel_location , unsigned int pml4_location);

void loader_main(unsigned int kernel_address , unsigned int kernel_sector_size) {
    clear_screen(0x07);
    print_debug_str(0 , 0 , 0x07 , "Loading the kernel..");
    
    // copy the kernel into the destination
    unsigned int kernel_size = kernel_sector_size*BYTES_PER_SECTOR;
    unsigned char *kernel_dest_ptr = (unsigned char *)KERNEL_LOAD_LOCATION;
    unsigned char *kernel_source_ptr = (unsigned char *)kernel_address;
    for(unsigned int i = 0; i < kernel_size; i++) {
        *kernel_dest_ptr = *kernel_source_ptr; 
        kernel_dest_ptr++; kernel_source_ptr++;
    }
    // setup PML4 table for long mode
    setup_pml4(PAGE_TABLE_LOCATION , 32); // 16GB max for now
    jump_to_longmode(KERNEL_LOAD_LOCATION , PAGE_TABLE_LOCATION);
    while(1) {
        ;
    }
}

void clear_screen(int color) {
	int i;
	unsigned char *vmem = (unsigned char *)0xB8000;
	for(i = 0; i < 80*25; i++) {
		*(vmem++) = 0x00;
		*(vmem++) = color;
	}
}

void print_debug_str(int x , int y , int color , const char *str) {
    unsigned char *vmem = (unsigned char *)0xB8000;
    vmem += 2*((80*y)+x);
    for(int i = 0; str[i] != 0; i++) {
        *(vmem++) = str[i];
        *(vmem++) = color;
    }
}

void debug_printf(int x , int y , int color , const char *fmt , ...) {
    char string[2048];
    va_list ap;
    va_start(ap , fmt);
    vsprintf(string , fmt , ap);
    print_debug_str(x , y , color , string);
    va_end(ap);
}
