;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;       Compact bootloader for x86_64 architecture       ;;
;;  Load the 32-bit Kernel Loader to designated location  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[ORG 0x7C00]
[BITS 16]

    jmp start

boot_drive:              db 0x00
kernel_loader_location:  dw 0x1000
kernel_loader_size:      dd 8
kernel_location:         dw 0x00
kernel_location_seg:     dw 0x1000
kernel_size:             dw 48

dap_address:   dw 0x700

start:
    mov ax , 0x00
    mov ds , ax

    xor ax , ax
    mov ss , ax
    mov sp , 0xFFF8
    mov bp , 0xFFF8

    ; Enable A20 gate
	mov eax , 0x2401
	int 0x15

    xor ecx , ecx

    mov [boot_drive] , dl

    ; read the disk content (sector #2)
    xor ax , ax
    mov es , ax
    mov bx , [kernel_loader_location]

    mov ah , 0x02
    mov al , byte[kernel_loader_size]
    mov ch , 0x00
    mov dh , 0x00
    mov cl , 2 ; sector location of kernel loader
    mov dl , [boot_drive]

    int 0x13

    jc failed

    xor ch , ch
    add cx , word[kernel_loader_size]
    dec cx
    
    ; use LBA
    mov ah , 0x42
    mov dl , [boot_drive]
    mov si , [dap_address]
    mov byte[es:si]     , 0x10 ; DAP structure size
    mov byte[es:si+1]   , 0x00
    mov di , [kernel_size]
    mov word[es:si+2]   , di
    mov di , word[kernel_location]
    mov word[es:si+4]   , di
    mov di , word[kernel_location_seg]
    mov word[es:si+6]   , di
    mov dword[es:si+8]  , ecx     ; sector location
    mov dword[es:si+12] , 0x00    ; (empty qword field)
    
    int 0x13

    jc failed
    
    cli

    ; switch to protect mode

    mov eax , cr0
    or eax , 0x01
    mov cr0 , eax
    
    lgdt [pmode_gdtr]

    ; store the kernel's location and size to the registers
    mov di , word[kernel_location]
    mov dx , word[kernel_location_seg]
    mov si , word[kernel_size]
    
    jmp 0x08:0x1000

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
    .offset: dd pmode_gdt

times (510-($-$$)) db 0x00
dw 0xAA55