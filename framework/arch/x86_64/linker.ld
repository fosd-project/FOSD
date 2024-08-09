OUTPUT_FORMAT(elf64-x86-64)
ENTRY(entry)

SECTIONS {
  . = 0x200000;
  .text 0x200000 : {
    *(.entry)
    *(.text .text.*)
  }
  .data : {
    *(.data .data.*)
  }
  .rodata : {
    *(.rodata .rodata.*)
  }
  .rodata1 : {
    *(.rodata1)
  }
  .bss : {
    *(.bss .bss.*)
  }
}