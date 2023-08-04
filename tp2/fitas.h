#ifndef _FITAS_H_
#define _FITAS_H_

#include <iostream>
#include "aluno.h"

#define NOME_FITAS "fitas/FITA_00.bin"
#define POSICAO_ALGARISMO_1 11
#define POSICAO_ALGARISMO_2 12
#define NUMERO_FITAS 40 // 2f
#define TAMANHO_INICIAL_BLOCO 20

typedef struct {
    int numeroAlunos;
    Aluno alunos[TAMANHO_INICIAL_BLOCO];
} Bloco;

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

void iniciaFitas(Fita*);

// void iniciaBloco(Bloco*);

void adicionaBloco(Fita*, Bloco*);

void imprimeFita(Fita*);

void imprimeFitas(Fita*);

void converteFitaTexto(Fita);

void flushFitas(Fita*);

#endif