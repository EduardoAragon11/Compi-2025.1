.data
print_fmt: .string "%ld \n"
.text
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
while_0:
 movq $1, %rax
 cmpq $0, %rax
 je endwhile_0
 movq $3, %rax
 movq %rax, %rsi
 leaq print_fmt(%rip), %rdi
 movl $0, %eax
 call printf@PLT
jmp endwhile_0
 jmp while_0
endwhile_0:
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
