.code32

.section .text

multiboot_header_begin:
    # 64-bit header align
    .align 8

    # Magic header value
    .long 0xe85250d6

    # Architecture type: protected mode 32-bit
    .long 0

    # Header size
    .long multiboot_header_end - multiboot_header_begin

    # Checksum
    .long -(0xe85250d6 + (multiboot_header_end - multiboot_header_begin))

    # End tag
    .short 0
    .short 0
    .long 8
multiboot_header_end:

.global _start64
.type _start64, @function
_start64:
    # Prints "Hi" to the screen. We can't call kernel_main because we are in
    # 32-bit mode here.
    movl $0x07690748,0xb8000
1:
    hlt
    jmp 1b

.size _start64, . - _start64

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
