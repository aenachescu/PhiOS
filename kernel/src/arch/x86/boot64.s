.section .text
.code32

multiboot_header_begin:
    .align 4
    .long 0xe85250d6
    .long 0
    .long multiboot_header_end - multiboot_header_begin
    .long -(0xe85250d6 + (multiboot_header_end - multiboot_header_begin))
multiboot_header_end:

.global _start64
.type _start64, @function
_start64:
    # 64-bit initialization should go here
    # and jump...
1:
    hlt
    jmp 1b

.size _start64, . - _start64

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
