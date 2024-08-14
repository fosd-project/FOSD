[BITS 32]

; section entry
SECTION .entry

global entry

extern loader_main

entry:
    mov ax , 0x10
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax
    mov ss , ax

    mov esp , 0x9FF8
    mov ebp , 0x9FF8

    ; convert the 16bit segment addressing to the physical address
    imul edx , 16
    add edi , edx

    push esi
    push edi
    call loader_main

    jmp $