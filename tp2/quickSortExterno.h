#ifndef _QUICK_SORT_EXTERNO_
#define _QUICK_SORT_EXTERNO_

#include "aluno.h"
#include "argumentos.h"

#define TAMANHO_AREA 20

typedef struct {
    Aluno alunos[TAMANHO_AREA];
    int numeroAlunos = 0;
} Area;

void ordenaQuickSort(char*, Argumentos*);

void quickSortExterno(FILE**, FILE**, FILE**, int, int);

void particao(FILE**, FILE**, FILE**, Area, int, int, int*, int*);

void fazAreaVazia(Area*);

void leSuperior(FILE**, Aluno*, int*, bool*);

void leInferior(FILE**, Aluno*, int*, bool*);

void inserirArea(Area*, Aluno*, int*);

void escreveMaximo(FILE**, Aluno, int*);

void escreveMinimo(FILE**, Aluno, int*);

void retiraMaximo(Area*, Aluno*, int*);

void retiraMinimo(Area*, Aluno*, int*);

int obterNumCelOcupadas(Area*);

void insereItem(Aluno*, Area*);

void retiraPrimeiro(Area *, Aluno *);

void retiraUltimo(Area *, Aluno *);


#endif