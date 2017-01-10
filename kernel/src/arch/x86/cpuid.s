.section .text
.global CPUID_IsSuported
.type CPUID_IsSuported, @function
.global CPUID_Call
.type CPUID_Call, @function

CPUID_IsSuported:
    pushfl
    pushfl

    movl $0x00200000, %eax
    xorl %eax, (%esp)

    popfl
    pushfl

    popl %ebx
    xorl (%esp), %ebx

    popfl

    andl %ebx, %eax

    ret

CPUID_Call:
    pushl %ebp
    movl %esp, %ebp
    sub $8, %esp
    movl %ebx, -4(%ebp)

    movl 8(%ebp), %eax
    movl 12(%ebp), %ebx
    movl 16(%ebp), %ecx
    movl 20(%ebp), %edx
    cpuid

    movl %edx, -8(%ebp)

    movl 24(%ebp), %edx
    movl %eax, (%edx)

    movl -8(%ebp), %edx

    movl 28(%ebp), %eax
    movl %ebx, (%eax)

    movl 32(%ebp), %eax
    movl %ecx, (%eax)

    movl 36(%ebp), %eax
    movl %edx, (%eax)

    movl -4(%ebp), %ebx
    movl %ebp, %esp
    popl %ebp

    ret
