.section .text

.global jumpToUserMode
.type jumpToUserMode, @function
jumpToUserMode:
    movw $0x23, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl $0x23
    movl $g_userStack, %eax
    pushl %eax

    pushfl
    pop %eax
    or $0x200, %eax
    pushl %eax

    pushl $0x1B
    pushl $user_main

    iret
