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

.global _start32
.type _start32, @function
_start32:
    # Set stack pointer.
    movl $stack_top, %esp

    # Reset EFLAGS
    pushl $0
    popf

    # Push pointer to multiboot structure
    pushl %ebx

    # Push magic value
    pushl %eax

    # Call main OS function
    call init_init32

    cli

    # Enable paging
    mov %cr0, %ecx
    or $0x80000001, %ecx
    mov %ecx, %cr0

    # Enter higher-half
    lea _higherHalf, %ecx
    add $0xBFF00000, %ecx
    jmp *%ecx             # NOTE: Must be absolute jump!

.global _higherHalf
.type _higherHalf, @function
_higherHalf:
    mov $stack_top, %esp
    add $0xBFF00000, %esp
    mov %esp, %ebp

    call kernel_main

    sti

1:
    hlt
    jmp 1b

.size _start32, . - _start32

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
