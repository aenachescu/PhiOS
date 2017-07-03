# Declare a multiboot header that marks the program as a kernel. These are magic
# values that are documented in the multiboot standard. The bootloader will
# search for this signature in the first 8 KiB of the kernel file, aligned at a
# 32-bit boundary. The signature is in its own section so the header can be
# forced to be within the first 8 KiB of the kernel file.
.section .multiboot_header
    multiboot_header_begin:
        .long 0xe85250d6
        .long 0
        .long multiboot_header_end - multiboot_header_begin
        .long -(0xe85250d6 + (multiboot_header_end - multiboot_header_begin))
        .short 0
        .short 0
         .long 8
    multiboot_header_end:
