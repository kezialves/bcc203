#ifndef _INTERCALACAO_BALANCEADA_H_
#define _INTERCALACAO_BALANCEADA_H_

#include "fitas.h"
#include "desempenho.h"
#include "argumentos.h"
#include "merge.h"

#define TAMANHO_MEMORIA_INTERNA 20

// Intercalação --------------------------------------------------

bool ordenaIntercalacaoBalanceada(Argumentos*, char*, Desempenho*);

bool intercala(Fita*, int, bool);

// Retorna se a intercalação deve continuar ou não
bool continuaIntercalacao(Fita*, bool);

// Retorna o menor elemento do vetor de alunos de acordo com a nota
int menorElemento(Aluno*, int*);

// Retorna a soma do vetor de controle
int somaVetorControle(int*);

// Retorna se a posição do vetor tem um aluno válido
bool temAlunoValido(int*);

// Blocos --------------------------------------------------

// Cria os blocos em fitas pelo método de ordenação interna
void criaBlocosOrdenacaoInterna(Fita*, Desempenho*, char*);

// Cria os blocos em fitas pelo método de substituição
void criaBlocosSubstituicao(Fita*, Desempenho*, char*);

// // Lê um bloco ???
// void leBloco(Bloco*, FILE*);

// Retorna o maior número de blocos entre as fitas
int maxBlocos(Fita*);

// Fitas --------------------------------------------------

// Volta o ponteiro de cada fita para seu início
void reiniciaPonteirosFitas(Fita*);

// Apaga as fitas atualmente de saída
void apagaFitasSaida(Fita*, bool);

// Fecha todas as 2f fitas abertas
void fechaFitas(Fita*);

#endif