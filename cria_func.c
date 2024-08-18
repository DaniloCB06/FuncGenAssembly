#include "cria_func.h"
#include <stdio.h>
#include <stdlib.h>

void cria_func(void *f, DescParam params[], int n, unsigned char codigo[]) {
  int contador;
  unsigned long aux, aux2;
  size_t offset = 0;
  char caracter;

  if (n < 1 || n > 3) {
    printf("Erro, numero de parametros deve ser no minimo 1 e maximo 3.\n");
    exit(1);
  }

  codigo[offset++] = 0x55; // pushq %rbp

  codigo[offset++] = 0x48; // movq %rsp, %rbp
  codigo[offset++] = 0x89;
  codigo[offset++] = 0xe5;

  codigo[offset++] = 0x48; // subq $32 da pilha
  codigo[offset++] = 0x83;
  codigo[offset++] = 0xec;
  codigo[offset++] = 0x20;

  codigo[offset++] = 0x48; // movq %rdi, -8(%rbp)
  codigo[offset++] = 0x89;
  codigo[offset++] = 0x7d;
  codigo[offset++] = 0xf8;

  codigo[offset++] = 0x48; // movq %rsi, -16(%rbp)
  codigo[offset++] = 0x89;
  codigo[offset++] = 0x75;
  codigo[offset++] = 0xf0;

  codigo[offset++] = 0x48; // movq %rdx, -24(%rbp)
  codigo[offset++] = 0x89;
  codigo[offset++] = 0x55;
  codigo[offset++] = 0xe8;

  if (params[0].orig_val != PARAM) {
    if (params[1].orig_val != PARAM && params[2].orig_val == PARAM) {
      codigo[offset++] = 0x48; // movq -8(%rbp), %rdx
      codigo[offset++] = 0x8b;
      codigo[offset++] = 0x55;
      codigo[offset++] = 0xf8;
    } else if (params[2].orig_val != PARAM && params[1].orig_val == PARAM) {
      codigo[offset++] = 0x48; // movq -8(%rbp), %rsi
      codigo[offset++] = 0x8b;
      codigo[offset++] = 0x75;
      codigo[offset++] = 0xf8;
    } else {
      codigo[offset++] = 0x48; // movq -8(%rbp), %rsi
      codigo[offset++] = 0x8b;
      codigo[offset++] = 0x75;
      codigo[offset++] = 0xf8;

      codigo[offset++] = 0x48; // movq -16(%rbp), %rdx
      codigo[offset++] = 0x8b;
      codigo[offset++] = 0x55;
      codigo[offset++] = 0xf0;
    }
  }

  else if (params[1].orig_val != PARAM && params[2].orig_val == PARAM &&
           params[0].orig_val == PARAM) {
    codigo[offset++] = 0x48; // movq -16(%rbp), %rdx
    codigo[offset++] = 0x8b;
    codigo[offset++] = 0x55;
    codigo[offset++] = 0xf0;
  }

  for (int i = 0; i < n; i++) {
    if (params[i].orig_val == PARAM) {

      if (params[i].tipo_val == INT_PAR) {

      } else if (params[i].tipo_val == PTR_PAR) {
      }

    } else if (params[i].orig_val == FIX) {

      switch (i) {
      case 0:
        caracter = 0xBF;
        break;

      case 1:
        caracter = 0xBE;
        break;

      case 2:
        caracter = 0xBA;
        break;
      }

      if (params[i].tipo_val == INT_PAR) {
        codigo[offset++] =
            caracter; // movl  $valor_parametro_amarrado, %registrador
        for (contador = 0; contador < sizeof(int); contador++) {
          codigo[offset++] = params[i].valor.v_int >> (8 * contador);
        }

      } else if (params[i].tipo_val == PTR_PAR) {
        codigo[offset++] = 0x48; // movabs $endereço_do_vetor_passado_na_main,
                                 // %registrador
        codigo[offset++] = caracter;
        aux2 = (unsigned long)params[i].valor.v_ptr;
        for (contador = 0; contador < sizeof(unsigned long); contador++) {
          codigo[offset++] = aux2 >> (8 * contador);
        }
      }
    } else if (params[i].orig_val == IND) {
      switch (i) {
      case 0:
        caracter = 0x39;
        break;

      case 1:
        caracter = 0x31;
        break;

      case 2:
        caracter = 0x11;
        break;
      }
      if (params[i].tipo_val == INT_PAR) {
        codigo[offset++] =
            0x48; /*movabs $endereço_do_ponteiro_em_params.valor.v_ptr, %rcx*/
        codigo[offset++] = 0xb9;
        aux = (unsigned long)params[i].valor.v_ptr;
        for (contador = 0; contador < sizeof(unsigned long); contador++) {
          codigo[offset++] = aux >> (8 * contador);
        }
        codigo[offset++] = 0x8b; // movl (%rcx), %edi

        codigo[offset++] = caracter;
      } else if (params[i].tipo_val == PTR_PAR) {
        codigo[offset++] =
            0x48; /*movabs $endereço_do_ponteiro_em_params.valor.v_ptr, %rcx*/
        codigo[offset++] = 0xb9;
        aux = (unsigned long)(params[i].valor.v_ptr);
        for (contador = 0; contador < sizeof(unsigned long); contador++) {
          codigo[offset++] = aux >> (8 * contador);
        }
        codigo[offset++] = 0x48; // movq (%rcx), %rdi
        codigo[offset++] = 0x8b;
        codigo[offset++] = caracter;
      }
    }
  }
  codigo[offset++] = 0x48; // movabs $0xEndereco_de_f, %rax
  codigo[offset++] = 0xb8;

  aux = (unsigned long)f;

  for (contador = 0; contador < sizeof(unsigned long); contador++) {
    codigo[offset++] = aux >> (contador * 8);
  }

  codigo[offset++] = 0xff; // call *%rax
  codigo[offset++] = 0xd0;

  // desfaz o registro de ativação e retorna

  codigo[offset++] = 0xC9; // leave
  codigo[offset++] = 0xC3; // ret

  for (int i = 0; i < offset; i++) {
    printf("Pos[%d]: %02x\n", i, codigo[i]);
  }
}
