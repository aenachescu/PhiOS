.section .text
.global GDT_Load32
.type GDT_Load32, @function

GDT_Load32:
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

.global IDT_Load32
.type IDT_Load32, @function

IDT_Load32:
    mov 4(%esp), %eax
    lidt (%eax)

    ret
