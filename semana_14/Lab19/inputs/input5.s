.data
print_fmt: .string "%ld \n"
.text
 .globl vacia
vacia:
 pushq %rbp
 movq %rsp, %rbp
 jmp .end_vacia
.end_vacia:
leave
ret
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 call vacia
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
