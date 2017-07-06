KERNEL_VMA = 0x100000;
KERNEL_LMA = 0x100000;
ALIGN_VAL = 0x1000;

OUTPUT_FORMAT("elf32-i386")
ENTRY(_start32)
SECTIONS
{
    . = KERNEL_LMA;

    linker_kernelStart = .;

    _boot = .;
    .boot BLOCK(4K) : ALIGN(4K)
    {
        CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot32.s.o (.text)
    }
    _eboot = .;

    _text = .;
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(EXCLUDE_FILE(CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot32.s.o) .text)
    }
    _etext = .;

    _rodata = .;
    .rodata BLOCK(4K) : ALIGN(4K)
    {
    	*(.rodata)
    }
    _erodata = .;

    _data = .;
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }
    _edata = .;

    _eh_frame = .;
    .eh_frame BLOCK(4K) : ALIGN(4K)
    {
        *(.eh_frame)
    }
    _eeh_frame = .;

    _bss = .;
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss)
        *(COMMON)
    }
    _ebss = .;

    linker_kernelEnd = .;

    .comment :
    {
        *(.comment)
    }
}
