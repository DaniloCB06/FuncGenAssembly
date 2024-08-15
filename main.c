#include "cria_func.h"
#include <stdio.h>

typedef int (*func_ptr)(int y, int z);

int mult(int *x, int y, int z) {
  return (*x) * y + z; 
}

int main(void) {
  DescParam params[3];
  func_ptr f_mult;
  int vetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int *ponteiro;
  ponteiro = vetor;
  int i;
  unsigned char codigo[500];

  params[0].tipo_val =
      PTR_PAR; 
  params[0].orig_val = IND; 
  params[0].valor.v_ptr = &ponteiro;

  params[1].tipo_val =
      INT_PAR; 
  params[1].orig_val = PARAM;

  params[2].tipo_val =
      INT_PAR;
  params[2].orig_val = PARAM;

  cria_func(mult, params, 3, codigo);
  f_mult = (func_ptr)codigo;

  for (i = 1, ponteiro = vetor; i <= 10; i++, ponteiro++) {
    printf("%d\n", f_mult(10, 20)); 
  }

  return 0;
}
