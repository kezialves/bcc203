#ifndef _FITAS_H_
#define _FITAS_H_

#include <iostream>

#define NOME_FITAS "FITA_01.txt"
#define POSICAO_ALGARISMO_1 5
#define POSICAO_ALGARISMO_2 6
#define NUMERO_FITAS 40

typedef struct{
    FILE* arquivo;
    int numeroBlocos = 0;
} Fita;

typedef Fita Fitas[NUMERO_FITAS];

#endif

