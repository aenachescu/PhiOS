ENTRY(bootstrap)
SECTIONS
{
    . = KERNEL_LMA;

        .bootstrap :
        {
            *(.multiboot_header)
            <path of bootstrap object> (.text)
        }

        . += KERNEL_VMA;

        .text : AT(ADDR(.text) - KERNEL_VMA)
        {
            _code = .;
            *(EXCLUDE_FILE(*<path of bootstrap object>) .text)
            *(.rodata*)
            . = ALIGN(4096);
        }

   .data : AT(ADDR(.data) - KERNEL_VMA)
   {
        _data = .;
        *(.data)
        . = ALIGN(4096);
   }

   .eh_frame : AT(ADDR(.eh_frame) - KERNEL_VMA)
   {
       _ehframe = .;
       *(.eh_frame)
        . = ALIGN(4096);
   }

   .bss : AT(ADDR(.bss) - KERNEL_VMA)
   {
       _bss = .;
       *(.bss)

       /*
        * You usually need to include generated COMMON symbols
        * under kernel BSS section or use gcc's -fno-common
        */

        *(COMMON)
       . = ALIGN(4096);
   }

   _end = .;

   /DISCARD/ :
   {
        *(.comment)
   }
}