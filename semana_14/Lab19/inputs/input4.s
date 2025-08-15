.data
print_fmt: .string "%ld \n"
.text
 .globl uno
uno:
 pushq %rbp
 movq %rsp, %rbp
 movq $1, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_uno:
leave
ret
 .globl dos
dos:
 pushq %rbp
 movq %rsp, %rbp
 movq $2, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 movq $3, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
.end_dos:
leave
ret
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 movq $20, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 call uno
 movq $21, %rax
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
