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
jmp endwhile_0
 jmp while_0
endwhile_0:
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
