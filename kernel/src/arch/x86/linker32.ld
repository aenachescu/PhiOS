KERNEL_VMA = 0x100000;
KERNEL_PMA = 0x100000;

OUTPUT_FORMAT("elf32-i386")
ENTRY(_start32)
SECTIONS
{
    /* Place kernel at 1 MiB */
    . = KERNEL_PMA;

    linker_kernelStart = .;

    /* Read-Only sections */
    linker_textStart = .;
    .text BLOCK(4K) : ALIGN(4K)
    {
        CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot32.s.o (.text)
        *(EXCLUDE_FILE(CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot32.s.o) .text)
    }
    linker_textEnd = .;

    linker_rodataStart = .;
    .rodata BLOCK(4K) : ALIGN(4K)
    {
    	*(.rodata)
    }
    linker_rodataEnd = .;

    /* Read-Write sections */
    linker_dataStart = .;
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }
    linker_dataEnd = .;

    _bss = .;
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss)
        *(COMMON)
    }
    _ebss = .;

    linker_kernelEnd = .;

    /* Discard useless sections */ 
    /DISCARD/ :
    {
        *(.note)
        *(.comment)
        *(.eh_frame) /* Add this if C++ support is needed */
    }
}
