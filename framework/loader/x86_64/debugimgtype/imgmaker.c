#include <stdio.h>
#include <stdlib.h>

#define BYTES_PER_SECTOR      512
#define FIXED_BOOTLOADER_SIZE 1*BYTES_PER_SECTOR

int main(int argc , char **argv) {
    if(argc != 5) {
        printf("Usage : imgmaker [output file] [bootloader] [kernel loader] [kernel]\n");
        return -1;
    }
    FILE *out = fopen(argv[1] , "wb");
    FILE *bootloader = fopen(argv[2] , "rb");
    FILE *kernel_loader = fopen(argv[3] , "rb");
    FILE *kernel = fopen(argv[4] , "rb");

    if(bootloader == 0x00) {
        printf("bootloader not found\n");
        return -1;
    }
    if(kernel_loader == 0x00) {
        printf("kernel loader not found\n");
        return -1;
    }
    if(kernel == 0x00) {
        printf("kernel not found\n");
        return -1;
    }
    
    fseek(kernel_loader , 0 , SEEK_END);
    fseek(kernel , 0 , SEEK_END);
    unsigned long kernel_loader_size = ftell(kernel_loader);
    unsigned long aligned_kernel_loader_size = (((unsigned long)(kernel_loader_size/BYTES_PER_SECTOR))+1)*BYTES_PER_SECTOR;
    unsigned long kernel_size = ftell(kernel);
    printf("kernel_loader_size = %ld(aligned : %ld)\n" , kernel_loader_size , aligned_kernel_loader_size);
    printf("kernel_size        = %ld\n" , kernel_size);
    fseek(kernel_loader , 0 , SEEK_SET);
    fseek(kernel , 0 , SEEK_SET);
    
    unsigned long i = 0;
    for(; i < FIXED_BOOTLOADER_SIZE; i++) {
        if(feof(bootloader)) break;
        unsigned char c = fgetc(bootloader);
        fputc(c , out);
    }
    for(i = 0; i < kernel_loader_size; i++)    { fputc(fgetc(kernel_loader) , out); }
    for(; i < aligned_kernel_loader_size; i++) { fputc(0 , out); }
    for(i = 0; i < kernel_size; i++) { fputc(fgetc(kernel) , out); }
    
    fclose(out);
    fclose(bootloader);
    fclose(kernel_loader);
    fclose(kernel);
    return 0;
}