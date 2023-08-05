#ifndef _FITAS_H_
#define _FITAS_H_

#include <iostream>

#include "aluno.h"
#include "desempenho.h"

#define NOME_FITAS "fitas/FITA_00.bin"
#define POSICAO_ALGARISMO_1 11 // primeiro número do nome da fita
#define POSICAO_ALGARISMO_2 12 // segundo número do nome da fita
#define NUMERO_FITAS 40 // 2f
#define TAMANHO_INICIAL_BLOCO 20

// Estrutura de dados dos blocos em memória principal
typedef struct {
    int numeroAlunos;
    Aluno alunos[TAMANHO_INICIAL_BLOCO];
} Bloco;

// Estrutura de dados das fitas
typedef struct {
    int vezesAcessadas; 
    FILE* arquivo;
    int numeroBlocos = 0;
} Fita;

// typedef Fita Fitas[2 * NUMERO_FITAS]; // 2f

// typedef struct {
//     Fita fitasEntrada[NUMERO_FITAS];
//     int fitasEntradaUsadas = 0;
    
//     Fita fitasSaida[NUMERO_FITAS];
//     int fitasSaidaUsadas = 0;
// } Fitas;

// void iniciaBloco(Bloco*);

// Inicia todas as f fitas
void iniciaFitas(Fita*);

// Adiciona um bloco em uma fita
void adicionaBloco(Fita*, Bloco*, Desempenho*);

// Adiciona um aluno em uma fita
void adicionaAluno(Fita, Aluno*);

// Retorna o maior número de blocos entre as fitas
int maxBlocos(Fita*, bool);

// Dá um fflush em todas as fitas
void flushFitas(Fita*);

// Volta o ponteiro de cada fita para seu início
void reiniciaPonteirosFitas(Fita*);

// Apaga as fitas atualmente de saída
void apagaFitasSaida(Fita*, bool);

// Fecha todas as 2f fitas abertas
void fechaFitas(Fita*);

// Impressão --------------------------------------------------

// Imprime todas as fitas
void imprimeFitas(Fita*);

// Imprime uma fita
void imprimeFita(Fita*);

// Auxiliar --------------------------------------------------

// Converte uma fita para arquivo texto
void converteFitaTexto(Fita);

#endif