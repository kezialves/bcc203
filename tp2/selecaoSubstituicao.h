#ifndef _SELECAO_SUBSTITUICAO_H_
#define _SELECAO_SUBSTITUICAO_H_

#include "fitas.h"
#include "heap.h"
#include "desempenho.h"

// Cria os blocos em fitas pelo método de substituição
void criaBlocosSelecaoSubstituicao(Fita*, char*, Desempenho*);

// Verifica se todos os alunos do heap estão marcados
bool todosMarcados(Heap*);

// Atribui false a todos os alunos do heap
void desmarcaTodos(Heap*);

#endif