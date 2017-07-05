.code32

multiboot_header_begin:
    .align 4
    .long 0xe85250d6
    .long 0
    .long multiboot_header_end - multiboot_header_begin
    .long -(0xe85250d6 + (multiboot_header_end - multiboot_header_begin))
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
    call kernel_main

1:
    hlt
    jmp 1b

.size _start32, . - _start32

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
