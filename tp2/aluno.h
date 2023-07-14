#ifndef _ALUNO_H_
#define _ALUNO_H_

#include <iostream>

#define STRING_TAM 100

using namespace std;

typedef struct{
    long numeroInscricao;
    double nota;
    char estado[STRING_TAM];
    char cidade[STRING_TAM];
    char curso[STRING_TAM];
} Aluno;

bool comparaAlunos(Aluno*, Aluno*);

#endif