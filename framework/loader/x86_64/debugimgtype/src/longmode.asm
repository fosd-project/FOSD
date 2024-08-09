[BITS 32]

SECTION .text

global jump_to_longmode

jump_to_longmode:
    push ebp
    mov ebp , esp

    mov edi , dword[ebp+8]  ; kernel jump address
    mov ebx , dword[ebp+12] ; pml4 table location

    cli 
    xor edx , edx
                            ; Switch to Long Mode
    mov eax , cr4           ; 1. Set PAE(Physical Address Extension) bit to 1 (5th bit)
                            ; If you set both PAE bit and PS bit, page size is set to 2MB.
    or eax , 0b1000100000   ; Also set OSFXSR to 1
    mov cr4 , eax
    
    mov eax , ebx
    mov cr3 , eax           ; 2. Set the location of the PML4 Entry (0x10000, check 140th line in current code)
    
    mov ecx , 0xC0000080
    rdmsr                   ; Access 0xC0000080 of MSR(Model Specific Register)

    or eax , 0b100000001    ; 3. Set LME(Long Mode Enable) bit and SCE(System Call Enable) bit (8th bit , 0th bit respectively)
                            ; - which both exist in MSR register
                            ; (System Call will be featured in this Operating System,
                            ;  that's why I'm currently setting SCE bit to 1)
    wrmsr
    
    mov eax , cr0
    or eax , 0x80000000     ; Set PG(Paging Enable) bit to 1
    mov cr0 , eax
    lgdt [longmode_gdtr]     ; Load long mode GDT so that we can use Data and Code segments without an issue

    mov ax , 0x10
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax
    mov ss , ax
    
    jmp 0x08:0x200000       ; Finally jump to long mode kernel (Main Kernel)

SECTION .data

longmode_gdt:
    dw 0x0000
    dw 0x0000
    db 0x00
    db 0x00
    db 0x00
    db 0x00

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xAF
    db 0x00

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xAF  ; Don't forget to set the L flag!!
    db 0x00
longmode_gdt_end:

longmode_gdtr:
    dw longmode_gdt_end-longmode_gdt
    dd longmode_gdt

longmode_idtr:
    dw 0x00
    dd 0x00