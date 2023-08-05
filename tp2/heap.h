#ifndef _HEAP_H_
#define _HEAP_H_

#include "aluno.h"
#include "fitas.h"
#include "desempenho.h"

// Estrutura de dados do aluno adaptado ao heap
typedef struct {
    Aluno aluno;
    bool marcado;
} AlunoHeap;

// Estrutura de dados do heap
typedef struct {
    AlunoHeap alunos[NUMERO_FITAS / 2];
    int numeroAlunos = 0;
} Heap; /* Hop */

// Constrói o heap :)
void heapConstroi(Heap*, Desempenho*);

// Refaz o heap :)
void heapRefaz(AlunoHeap*, int, int, Desempenho*);

// Compara dois AlunoHeap levando em consideração a marcação
bool comparaAlunosHeap(AlunoHeap*, AlunoHeap*, Desempenho*);

// Imprime o heap :)
void imprimeHeap(Heap*);

#endif