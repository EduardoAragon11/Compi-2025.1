.data
print_fmt: .string "%ld \n"
.text
 .globl terminar
terminar:
 pushq %rbp
 movq %rsp, %rbp
 jmp .end_terminar
.end_terminar:
leave
ret
 .globl main
main:
 pushq %rbp
 movq %rsp, %rbp
 call terminar
 movq $0, %rax
 jmp .end_main
.end_main:
leave
ret
.section .note.GNU-stack,"",@progbits
