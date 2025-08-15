.data
print_fmt: .string "%ld \n"
.text
.globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $6, %rax
 pushq %rax
 movq $2, %rax
 movq %rax, %rcx
 popq %rax
 movq %rax, %rcx
 movq $1, %rax
 addq %rcx, %rax
 movq %rax, -8(%rbp)
 movq -8(%rbp), %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
