#include <stdio.h>
#include <stdlib.h> /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h> /* getopt */
#include <time.h>
#include <likwid.h>

#include "matriz.h"
#include "utils.h"

/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ <ordem> ] \n", progname);
  exit(1);
}
/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */

int main(int argc, char *argv[])
{
  LIKWID_MARKER_INIT;
  int n = DEF_SIZE;

  MatRow mRow_1, mRow_2, resMat;
  Vetor vet, res;

  /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

  if (argc < 2)
    usage(argv[0]);

  n = atoi(argv[1]);

  /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */

  srandom(20232);

  res = geraVetor(n, 0); // (real_t *) malloc (n*sizeof(real_t));
  resMat = geraMatRow(n, n, 1);

  mRow_1 = geraMatRow(n, n, 0);
  mRow_2 = geraMatRow(n, n, 0);

  vet = geraVetor(n, 0);

  if (!res || !resMat || !mRow_1 || !mRow_2 || !vet)
  {
    fprintf(stderr, "Falha em alocação de memória !!\n");
    liberaVetor((void *)mRow_1);
    liberaVetor((void *)mRow_2);
    liberaVetor((void *)resMat);
    liberaVetor((void *)vet);
    liberaVetor((void *)res);
    exit(2);
  }

#ifdef _DEBUG_
  prnMat(mRow_1, n, n);
  prnMat(mRow_2, n, n);
  prnVetor(vet, n);
  printf("=================================\n\n");
#endif /* _DEBUG_ */

  /* MATxVET sem otimizacao */
  LIKWID_MARKER_START("MULT_MAT_VET");
  double tempoMatVet = timestamp();
  multMatVet(mRow_1, vet, n, n, res);
  tempoMatVet = timestamp() - tempoMatVet;
  LIKWID_MARKER_STOP("MULT_MAT_VET");

  printf("tempo matvet sem otimizacao: %f\n", tempoMatVet);

  /* MATxMAT sem otimizacao */
  LIKWID_MARKER_START("MULT_MAT_MAT");
  double tempoMatMat = timestamp();
  multMatMat(mRow_1, mRow_2, n, resMat);
  tempoMatMat = timestamp() - tempoMatMat;
  LIKWID_MARKER_STOP("MULT_MAT_MAT");

  printf("tempo matmat sem otimizacao: %f\n", tempoMatMat);

  /* MATxVET com otimizacao */
  LIKWID_MARKER_START("MULT_MAT_VET_UNROLL_JAM");
  double tempoMatVetOtimizado = timestamp();
  multMatVetUnrollJam(mRow_1, vet, n, n, res);
  tempoMatVetOtimizado = timestamp() - tempoMatVetOtimizado;
  LIKWID_MARKER_STOP("MULT_MAT_VET_UNROLL_JAM");

  printf("tempo matvet com otimizacao: %f\n", tempoMatVetOtimizado);

  /* MATxMAT com otimizacao */
  LIKWID_MARKER_START("MULT_MAT_MAT_UNRJAM_BLOCKING");
  double tempoMatMatOtimizado = timestamp();
  multMatMatUnrollJamBk(mRow_1, mRow_2, n, resMat);
  tempoMatMatOtimizado = timestamp() - tempoMatMatOtimizado;
  LIKWID_MARKER_STOP("MULT_MAT_MAT_UNRJAM_BLOCKING");

  printf("tempo matmat com otimizacao: %f\n", tempoMatMatOtimizado);

#ifdef _DEBUG_
  prnVetor(res, n);
  prnMat(resMat, n, n);
#endif /* _DEBUG_ */

  liberaVetor((void *)mRow_1);
  liberaVetor((void *)mRow_2);
  liberaVetor((void *)resMat);
  liberaVetor((void *)vet);
  liberaVetor((void *)res);

  LIKWID_MARKER_CLOSE;

  return 0;
}
