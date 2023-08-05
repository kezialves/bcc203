#ifndef _ALUNO_H_
#define _ALUNO_H_

#include <iostream>

#include "desempenho.h"

#define MAX_ESTADO 3 // tamanho máximo da sigla do estado contando o \0
#define MAX_CIDADE 100 // tamanho máximo do nome da cidade
#define MAX_CURSO 100 // tamanho máximo do nome do curso

using namespace std;

// Estrutura de dados dos alunos
typedef struct{
    long numeroInscricao;
    double nota;
    char estado[MAX_ESTADO];
    char cidade[MAX_CIDADE];
    char curso[MAX_CURSO];
} Aluno;

// Compara dois alunos de acordo com a nota
bool comparaAlunos(Aluno*, Aluno*, Desempenho*);

#endif