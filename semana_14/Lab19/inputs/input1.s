.data
print_fmt: .string "%ld \n"
.text
 .globl mostrar
mostrar:
 pushq %rbp
 movq %rsp, %rbp
 movq $10, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_mostrar:
leave
ret
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 call mostrar
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
