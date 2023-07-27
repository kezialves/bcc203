#ifndef _QUICK_SORT_EXTERNO_
#define _QUICK_SORT_EXTERNO_

#include "aluno.h"
#include "argumentos.h"
#include "desempenho.h"

#define TAMANHO_AREA 20

typedef struct {
    Aluno alunos[TAMANHO_AREA];
    int numeroAlunos = 0;
} Area;

void ordenaQuickSort(char*, Argumentos *, Desempenho *);

void quickSortExterno(FILE**, FILE**, FILE**, int, int, Desempenho *);

void particao(FILE**, FILE**, FILE**, Area, int, int, int*, int*, Desempenho *);

void fazAreaVazia(Area*);

void leSuperior(FILE**, Aluno*, int*, bool*, Desempenho *);

void leInferior(FILE**, Aluno*, int*, bool*, Desempenho *);

void inserirArea(Area*, Aluno*, int*, Desempenho *);

void escreveMaximo(FILE**, Aluno, int*, Desempenho *);

void escreveMinimo(FILE**, Aluno, int*, Desempenho *);

void retiraMaximo(Area*, Aluno*, int*);

void retiraMinimo(Area*, Aluno*, int*);

int obterNumCelOcupadas(Area*);

void insereItem(Aluno*, Area*, Desempenho *);

void retiraPrimeiro(Area *, Aluno *);

void retiraUltimo(Area *, Aluno *);


#endif