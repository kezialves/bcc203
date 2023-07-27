#ifndef _MERGE_H_
#define _MERGE_H_

#include "aluno.h"

void mergeSort(Aluno *alunos, int l, int r);
void merge(Aluno *alunos, int l, int m, int r);
Aluno *aloca(int tamanho);
void desaloca(Aluno *vetor);

#endif