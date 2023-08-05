#ifndef _MERGE_H_
#define _MERGE_H_

#include "aluno.h"
#include "desempenho.h"

// Função recursiva do MergeSort
void mergeSort(Aluno *alunos, int l, int r, Desempenho*);

// Função iterativa do MergeSort
void merge(Aluno *alunos, int l, int m, int r, Desempenho*);

// Aloca um vetor de alunos para a divisão
Aluno *aloca(int tamanho);

// Desaloca o vetor de alunos alocado na divisão
void desaloca(Aluno *vetor);

#endif