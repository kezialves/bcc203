#ifndef _ORDENACAO_INTERNA_H_
#define _ORDENACAO_INTERNA_H_

#include "aluno.h"
#include "fitas.h"
#include "desempenho.h"
#include "merge.h"

// Cria os blocos em fitas pelo método de ordenação interna
void criaBlocosOrdenacaoInterna(Fita*, Desempenho*, char*);

#endif