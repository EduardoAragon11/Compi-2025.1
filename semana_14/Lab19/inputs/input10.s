.data
print_fmt: .string "%ld \n"
.text
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 subq $8, %rsp
 movq $0, %rax
 movq %rax, -8(%rbp)
while_0:
 movq $1, %rax
 cmpq $0, %rax
 je endwhile_0
 movq -8(%rbp), %rax
 pushq %rax
 movq $0, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 sete %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_1
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 addq %rcx, %rax
 movq %rax, -8(%rbp)
jmp endwhile_0
 jmp endif_1
 else_1:
endif_1:
 movq -8(%rbp), %rax
 pushq %rax
 movq $1, %rax
 movq %rax, %rcx
 popq %rax
 cmpq %rcx, %rax
 movl $0, %eax
 sete %al
 movzbq %al, %rax
 cmpq $0, %rax
 je else_2
jmp endwhile_0
 jmp endif_2
 else_2:
endif_2:
 jmp while_0
endwhile_0:
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
