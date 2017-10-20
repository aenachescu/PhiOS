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
    .text BLOCK(4K) : ALIGN(4K)
    {
        linker_textStart = .;

        CMakeFiles/phios.elf.dir/kernel/src/arch/x86/descriptors32.s.o (.text)
        CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot32.s.o (.text)
        *(EXCLUDE_FILE(CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot32.s.o CMakeFiles/phios.elf.dir/kernel/src/arch/x86/descriptors32.s.o) .text)

        linker_textEnd = .;
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        linker_rodataStart = .;

    	*(.rodata)

        linker_rodataEnd = .;
    }

    /* Read-Write sections */
    .data BLOCK(4K) : ALIGN(4K)
    {
        linker_dataStart = .;

        *(.data)

        . = ALIGN(4);
        linker_gotStart = .;
        KEEP(*(.got))
        KEEP(*(.got.plt))
        linker_gotEnd = .;

        linker_dataEnd = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        linker_bssStart = .;

        *(.bss)
        *(COMMON)

        linker_bssEnd = .;
    }

    linker_kernelEnd = .;

    /* Discard useless sections */ 
    /DISCARD/ :
    {
        *(.note)
        *(.comment)
        *(.eh_frame) /* Add this if C++ support is needed */
    }
}
