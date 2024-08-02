OUTPUT_FORMAT(elf32-i386)
ENTRY(loader_main)
SECTIONS {
  . = 0x4000;
  .text 0x4000 : {
    *(.text .stub .text.* .gnu.linkonce.t.*)
  }
  .data : {
    *(.data .data.* .gnu.linkonce.d.*)
  }
  .rodata : {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  .rodata1 : {
    *(.rodata1)
  }
  .bss : {
    *(.bss .bss.* .gnu.linkonce.b.*)
  }
}