.section .text
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

    jmp $0x08, $flush
flush:
    ret

.global IDT32_Load
.type IDT32_Load, @function

IDT32_Load:
    mov 4(%esp), %eax
    lidt (%eax)

    ret
