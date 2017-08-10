.section .text
.code32

.global GDT32_Load
.type GDT32_Load, @function

GDT32_Load:
    mov 4(%esp), %eax
    lgdt (%eax)

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # jmp $0x08, $flush
    # descriptor32.s it's mapped first, so the flush is somewhat constant...
    # that trick is needed because the kernel is pic...
    jmp $0x08, $0xC000001c
flush:
    ret

.global IDT32_Load
.type IDT32_Load, @function

IDT32_Load:
    mov 4(%esp), %eax
    lidt (%eax)

    ret

.global TSS32_Load
.type TSS32_Load, @function

TSS32_Load:
    movw $0x2B, %ax
    ltr %ax

    ret
