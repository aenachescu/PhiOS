.section .text
.code32

multiboot_header_begin:
    /*
        TODO: here should be the multiboot header
        Using 32bit Multiboot 2 header the OS doesn't start.
    */
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
