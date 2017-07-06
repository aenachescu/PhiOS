.section .text
.code32

.macro ISR_NOERRCODE_32 n
    .global isr_32_\n
    .type isr_32_\n, @function
    isr_32_\n:
        cli
        pushl $0
        pushl $\n
        jmp isr_common_stub_32
.endm

.macro ISR_ERRCODE_32 n
    .global isr_32_\n
    .type isr_32_\n, @function
    isr_32_\n:
        cli
        pushl $\n
        jmp isr_common_stub_32
.endm

.macro IRQ_32 n m
  .global irq_32_\n
  .type irq_32_\n, @function
  irq_32_\n:
    cli
    pushl $0
    pushl $\m
    jmp irq_common_stub_32
.endm

ISR_NOERRCODE_32 0
ISR_NOERRCODE_32 1
ISR_NOERRCODE_32 2
ISR_NOERRCODE_32 3
ISR_NOERRCODE_32 4
ISR_NOERRCODE_32 5
ISR_NOERRCODE_32 6
ISR_NOERRCODE_32 7
ISR_ERRCODE_32   8
ISR_NOERRCODE_32 9
ISR_ERRCODE_32   10
ISR_ERRCODE_32   11
ISR_ERRCODE_32   12
ISR_ERRCODE_32   13
ISR_ERRCODE_32   14
ISR_NOERRCODE_32 15
ISR_NOERRCODE_32 16
ISR_NOERRCODE_32 17
ISR_NOERRCODE_32 18
ISR_NOERRCODE_32 19
ISR_NOERRCODE_32 20
ISR_NOERRCODE_32 21
ISR_NOERRCODE_32 22
ISR_NOERRCODE_32 23
ISR_NOERRCODE_32 24
ISR_NOERRCODE_32 25
ISR_NOERRCODE_32 26
ISR_NOERRCODE_32 27
ISR_NOERRCODE_32 28
ISR_NOERRCODE_32 29
ISR_NOERRCODE_32 30
ISR_NOERRCODE_32 31
ISR_NOERRCODE_32 128

IRQ_32   0,    32
IRQ_32   1,    33
IRQ_32   2,    34
IRQ_32   3,    35
IRQ_32   4,    36
IRQ_32   5,    37
IRQ_32   6,    38
IRQ_32   7,    39
IRQ_32   8,    40
IRQ_32   9,    41
IRQ_32  10,    42
IRQ_32  11,    43
IRQ_32  12,    44
IRQ_32  13,    45
IRQ_32  14,    46
IRQ_32  15,    47

.extern IDT_irqHandler32
.extern IDT_isrHandler32

isr_common_stub_32:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esp
    pushl %ebp
    pushl %esi
    pushl %edi

    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs

    mov %cr2, %eax
    pushl %eax

    mov %cr3, %eax
    pushl %eax

    lea (%esp), %eax
    pushl %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call IDT32_isrHandler

    popl %ebx

    popl %ebx
    mov %ebx, %cr3

    popl %ebx
    mov %ebx, %cr2

    popw %gs
    popw %fs
    popw %es
    popw %ds

    popl %edi
    popl %esi
    popl %ebp
    popl %esp
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax

    add $8, %esp
    sti
    iret

irq_common_stub_32:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esp
    pushl %ebp
    pushl %esi
    pushl %edi

    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs

    mov %cr2, %eax
    pushl %eax

    mov %cr3, %eax
    pushl %eax

    lea (%esp), %eax
    pushl %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call IDT32_irqHandler

    popl %ebx

    popl %ebx
    mov %ebx, %cr3

    popl %ebx
    mov %ebx, %cr2

    popw %gs
    popw %fs
    popw %es
    popw %ds

    popl %edi
    popl %esi
    popl %ebp
    popl %esp
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax

    add $8, %esp
    sti
    iret
