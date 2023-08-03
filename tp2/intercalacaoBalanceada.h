#ifndef _INTERCALACAO_BALANCEADA_H_
#define _INTERCALACAO_BALANCEADA_H_

#include "fitas.h"
#include "desempenho.h"
#include "argumentos.h"
#include "merge.h"

#define TAMANHO_MEMORIA_INTERNA 20

bool ordenaIntercalacaoBalanceada(Argumentos*, char*, Desempenho*);

void criaBlocosOrdenacaoInterna(Fita*, Desempenho*, char*);

void criaBlocosSubstituicao(Fita*, Desempenho*, char*);

void leBloco(Bloco*, FILE*);

void adicionaBloco(Fita*, Bloco*);

bool intercala(Fita*, int, bool fitaIntercalada);

int menorElemento(Bloco*, int*);

int maxBlocos(Fita *);

int totalAlunosBlocoAIntercalar(Fita *fitas);

void reiniciaPonteirosFitas(Fita *);

int somaVetorControle(int *);

bool continuaIntercalacao(Fita *);

void fechaFitas (Fita* fitas);

#endif