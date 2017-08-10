KERNEL_VMA = 0x100000;
KERNEL_PMA = 0x100000;

OUTPUT_FORMAT("elf64-x86-64")
ENTRY(_start64)
SECTIONS
{
    /* Place kernel at 1 MiB */
    . = KERNEL_PMA;

    linker_kernelStart = .;

    /* Read-Only sections */
    .text BLOCK(4K) : ALIGN(4K)
    {
        linker_textStart = .;

        CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot64.s.o (.text)
        *(EXCLUDE_FILE(CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot64.s.o) .text)

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
