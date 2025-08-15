.data
print_fmt: .string "%ld \n"
.text
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
while_0:
 movq $0, %rax
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
