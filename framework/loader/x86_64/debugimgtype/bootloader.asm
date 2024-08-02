;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;       Compact bootloader for x86_64 architecture       ;;
;;  Load the 32-bit Kernel Loader to designated location  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]

    jmp 0x07C0:start

boot_drive:              db 0x00
kernel_loader_location:  dw 0x4000
kernel_loader_size:      dd 4

start:
    mov ax , 0x07C0
    mov ds , ax

    xor ax , ax
    mov ss , ax
    mov sp , 0x9FF8
    mov bp , 0x9FF8

    mov [boot_drive] , dl

    ; read the disk content (sector #2)
    xor ax , ax
    mov es , ax
    mov bx , [kernel_loader_location]

    mov ah , 0x02
    mov al , [kernel_loader_size]
    mov ch , 0x00
    mov dh , 0x00
    mov cl , 2 ; sector location of kernel loader
    mov dl , [boot_drive]
    
    int 0x13

    jc failed

	mov eax , 0x2401
	int 0x15

    ; switch to protect mode
    lgdt [pmode_gdtr]
    
    cli

    mov eax , cr0
    or eax , 0x01
    mov cr0 , eax

    jmp 0x08:(pmem_entry+0x7C00)

failed:
    mov ax , 0xB800
    mov ds , ax

    mov byte[0x00] , 'E'
    mov byte[0x01] , 0x04
    mov byte[0x02] , 'r'
    mov byte[0x03] , 0x04
    mov byte[0x04] , 'r'
    mov byte[0x05] , 0x04
    mov byte[0x06] , 'o'
    mov byte[0x07] , 0x04
    mov byte[0x08] , 'r'
    mov byte[0x09] , 0x04
    mov byte[0x0A] , '!'
    mov byte[0x0B] , 0x04
    jmp $

pmode_gdt:
    dw 0x0000
    dw 0x0000
    db 0x00
    db 0x00
    db 0x00
    db 0x00

    ; code segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00

    ; data segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00
pmode_gdt_end:

pmode_gdtr:
    .size: dw pmode_gdt_end-pmode_gdt
    .offset: dd pmode_gdt+0x7C00

[BITS 32]

pmem_entry:
    mov ax , 0x10
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax
    mov ss , ax
    
    jmp 0x08:0x4000

    jmp $

times (510-($-$$)) db 0x00
dw 0xAA55