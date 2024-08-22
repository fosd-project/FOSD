OUTPUT_FORMAT(elf64-x86-64)
ENTRY(entry)

INCLUDE arch_configurations.ld

SECTIONS {
  . = _KERNEL_LOAD_LOCATION;
  .entry _KERNEL_LOAD_LOCATION : {
    *(.entry)
  }
  .text : {
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