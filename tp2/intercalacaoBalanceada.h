#ifndef _INTERCALACAO_BALANCEADA_H_
#define _INTERCALACAO_BALANCEADA_H_

#include "fitas.h"
#include "desempenho.h"
#include "argumentos.h"
#include "merge.h"

bool ordenaIntercalacaoBalanceada(Argumentos*, char*, Desempenho*);

void criaBlocosOrdenacaoInterna(Fitas*, Desempenho*, char*);

void criaBlocosSubstituicao(Fitas*, Desempenho*, char*);

void leBloco(Bloco*, FILE*);

void adicionaBloco(Fitas*, Bloco*);


#endif