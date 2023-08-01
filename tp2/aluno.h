#ifndef _ALUNO_H_
#define _ALUNO_H_

#include <iostream>

#define MAX_ESTADO 3 // tamanho máximo da sigla do estado contando o \0
#define MAX_CIDADE 100 // tamanho máximo do nome da cidade
#define MAX_CURSO 100 // tamanho máximo do nome do curso

using namespace std;

typedef struct{
    long numeroInscricao;
    double nota;
    char estado[MAX_ESTADO];
    char cidade[MAX_CIDADE];
    char curso[MAX_CURSO];
} Aluno;

bool comparaAlunos(Aluno*, Aluno*);

#endif