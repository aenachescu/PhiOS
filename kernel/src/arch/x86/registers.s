# Used to write/read CPU registers
.code32
.section .text
.global readCR0
.type readCR0, @function
readCR0:
    movl %cr0, %eax
    ret

.section .text
.global writeCR0
.type writeCR0, @function
writeCR0:
    pushl %ebp
    movl %esp, %ebp
    movl 0x8(%ebp), %eax
    movl %eax, %cr0
    popl %ebp
    ret

.global readCR3
.type readCR3, @function
readCR3:
    movl %cr3, %eax
    ret

.section .text
.global writeCR3
.type writeCR3, @function
writeCR3:
    pushl %ebp
    movl %esp, %ebp
    movl 0x8(%ebp), %eax
    movl %eax, %cr3
    popl %ebp
    ret