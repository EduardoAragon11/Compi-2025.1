.data
print_fmt: .string "%ld \n"
.text
 .globl mensaje
mensaje:
 pushq %rbp
 movq %rsp, %rbp
 movq $1, %rax
 cmpq $0, %rax
 je else_0
 movq $2, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
 jmp endif_0
 else_0:
endif_0:
.end_mensaje:
leave
ret
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 call mensaje
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
