#ifndef _FITAS_H_
#define _FITAS_H_

#include <iostream>
#include "aluno.h"

#define NOME_FITAS "FITA_00.bin"
#define POSICAO_ALGARISMO_1 5
#define POSICAO_ALGARISMO_2 6
#define NUMERO_FITAS 20 // f
#define TAMANHO_INICIAL_BLOCO 20

typedef struct {
    int numeroAlunos;
    Aluno *alunos;
} Bloco;


typedef struct {
    FILE* arquivo;
    int numeroBlocos = 0;
} Fita;

typedef Fita Fitas[2 * NUMERO_FITAS]; // 2f


// typedef struct {
//     Fita fitasEntrada[NUMERO_FITAS];
//     int fitasEntradaUsadas = 0;
    
//     Fita fitasSaida[NUMERO_FITAS];
//     int fitasSaidaUsadas = 0;
// } Fitas;

void iniciaFitas(Fitas*);

void iniciaBloco(Bloco*);

void adicionaBloco(Fita *fita, Bloco *bloco);

#endif