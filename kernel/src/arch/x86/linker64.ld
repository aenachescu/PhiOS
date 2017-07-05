KERNEL_VMA = 0x100000;
KERNEL_LMA = 0x100000;
ALIGN_VAL = 0x1000;

OUTPUT_FORMAT("elf32-x86-64")
ENTRY(_start64)
SECTIONS
{
    . = KERNEL_LMA;

    linker_kernelStart = .;

    _boot = .;
    .boot :
    {
        CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot64.s.o (.text)
    }
    _eboot = .;

    _text = .;
    .text :
    {
        *(EXCLUDE_FILE(CMakeFiles/phios.elf.dir/kernel/src/arch/x86/boot64.s.o) .text)
    }
    _etext = .;

    _data = .;
    .data :
    {
        *(.data)
    }
    _edata = .;

    _eh_frame = .;
    .eh_frame :
    {
        *(.eh_frame)
    }
    _eeh_frame = .;

    _bss = .;
    .bss :
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
