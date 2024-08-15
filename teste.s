/*
movabs $1, %rdx
movq (%rdx), %rdi
*/


.data
.text
.globl cria_func

cria_func:
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq %rsi, -8(%rbp)
  movq %rdx, -16(%rbp)

  movq -8(%rbp), %rbx
  movq -8(%rbp), %rdi
  movq -16(%rbp), %rbx

  leave
  ret


