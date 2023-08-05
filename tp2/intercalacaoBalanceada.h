#ifndef _INTERCALACAO_BALANCEADA_H_
#define _INTERCALACAO_BALANCEADA_H_

#include "fitas.h"
#include "desempenho.h"
#include "argumentos.h"
#include "merge.h"

#define TAMANHO_MEMORIA_INTERNA 20

bool ordenaIntercalacaoBalanceada(Argumentos*, char*, Desempenho*, Desempenho*);

bool intercala(Fita*, int, bool, Desempenho*);

// Retorna se a intercalação deve continuar ou não
bool continuaIntercalacao(Fita*, bool);

// Retorna o menor elemento do vetor de alunos de acordo com a nota
int menorElemento(Aluno*, int*, Desempenho*);

// Retorna a soma do vetor de controle
int somaVetorControle(int*);

// Retorna se a posição do vetor tem um aluno válido
bool temAlunoValido(int*);

#endif