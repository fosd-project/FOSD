[BITS 64]

SECTION .entry

extern kernel_init
global entry

entry:
    mov ax , 0x10
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax
    mov ss , ax

    call kernel_init

    jmp $