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

  int n = DEF_SIZE;

  MatRow mRow_1, mRow_2, resMat1, resMat2;
  Vetor vet, res1, res2;

  /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

  if (argc < 2)
    usage(argv[0]);

  n = atoi(argv[1]);

  /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */

  srandom(20232);

  res1 = geraVetor(n, 1);
  res2 = geraVetor(n, 1);
  resMat1 = geraMatRow(n, n, 1);
  resMat2 = geraMatRow(n, n, 1);

  mRow_1 = geraMatRow(n, n, 0);
  mRow_2 = geraMatRow(n, n, 0);

  vet = geraVetor(n, 0);

  if (!res1 || !res2 || !resMat1 || !resMat2 || !mRow_1 || !mRow_2 || !vet)
  {
    fprintf(stderr, "Falha em alocação de memória !!\n");
    liberaVetor((void *)mRow_1);
    liberaVetor((void *)mRow_2);
    liberaVetor((void *)resMat1);
    liberaVetor((void *)resMat2);
    liberaVetor((void *)vet);
    liberaVetor((void *)res1);
    liberaVetor((void *)res2);
    exit(2);
  }

#ifdef _DEBUG_
  prnMat(mRow_1, n, n);
  prnMat(mRow_2, n, n);
  prnMat(mRow_3, n, n);
  prnMat(mRow_4, n, n);

  prnVetor(vet1, n);
  prnVetor(vet2, n);
  printf("=================================\n\n");
#endif /* _DEBUG_ */

  LIKWID_MARKER_INIT;

  /* MATxVET sem otimizacao */
  LIKWID_MARKER_START("MULT_MAT_VET");
  double tempo = timestamp();
  multMatVet(mRow_1, vet, n, n, res1);
  tempo = timestamp() - tempo;
  LIKWID_MARKER_STOP("MULT_MAT_VET");

  printf("%d,%f,", n, tempo);

  /* MATxMAT sem otimizacao */
  LIKWID_MARKER_START("MULT_MAT_MAT");
  tempo = timestamp();
  multMatMat(mRow_1, mRow_2, n, resMat1);
  tempo = timestamp() - tempo;
  LIKWID_MARKER_STOP("MULT_MAT_MAT");

  printf("%f,", tempo);

  /* MATxVET com otimizacao */
  LIKWID_MARKER_START("MULT_MAT_VET_UNROLL_JAM");
  tempo = timestamp();
  multMatVetUnrollJam(mRow_1, vet, n, n, res2);
  tempo = timestamp() - tempo;
  LIKWID_MARKER_STOP("MULT_MAT_VET_UNROLL_JAM");

  printf("%f,", tempo);

  /* MATxMAT com otimizacao */
  LIKWID_MARKER_START("MULT_MAT_MAT_UNRJAM_BLOCKING");
  tempo = timestamp();
  multMatMatUnrollJamBk(mRow_1, mRow_2, n, resMat2);
  tempo = timestamp() - tempo;
  LIKWID_MARKER_STOP("MULT_MAT_MAT_UNRJAM_BLOCKING");

  printf("%f\n", tempo);

  LIKWID_MARKER_CLOSE;

#ifdef _DEBUG_
  prnVetor(res1, n);
  prnMat(resMat1, n, n);
  prnVetor(res2, n);
  prnMat(resMat2, n, n);
#endif /* _DEBUG_ */

  liberaVetor((void *)mRow_1);
  liberaVetor((void *)mRow_2);
  liberaVetor((void *)resMat1);
  liberaVetor((void *)resMat2);
  liberaVetor((void *)vet);
  liberaVetor((void *)res1);
  liberaVetor((void *)res2);

  return 0;
}