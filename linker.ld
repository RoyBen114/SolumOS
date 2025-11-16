ENTRY(boot_start)

SECTIONS
{
    . = 0x100000;

    .multiboot2 BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot2)
    }
    
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
        *(.text.*)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
        *(.rodata.*)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
        *(.data.*)
    }
    
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }
    
    /DISCARD/ : {
        *(.note*)
        *(.iplt*)
        *(.igot*)
        *(.rel*)
        *(.comment)
    }
}