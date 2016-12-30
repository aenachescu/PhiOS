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
    movl 4(%esp), %eax
    movl 8(%esp), %ebx
    movl 12(%esp), %ecx
    movl 16(%esp), %edx
    cpuid

    pushl %edx

    movl 24(%esp), %edx
    movl %eax, (%edx)

    popl %edx

    movl 24(%esp), %eax
    movl %ebx, (%eax)

    movl 28(%esp), %eax
    movl %ecx, (%eax)

    movl 32(%esp), %eax
    movl %edx, (%eax)

    ret
