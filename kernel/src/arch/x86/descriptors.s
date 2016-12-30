.section .text
.global GDT_Load
.type GDT_Load, @function

GDT_Load:
    ret

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
