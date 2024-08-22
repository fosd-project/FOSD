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

    ; Temporary stack
    mov rbp , 0x4FFFF8
    mov rsp , 0x4FFFF8

    call kernel_init

    jmp $