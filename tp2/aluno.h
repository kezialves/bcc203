#ifndef _ALUNO_H_
#define _ALUNO_H_

#include <iostream>

using namespace std;


typedef struct{
    long numeroInscricao;
    double nota;
    string estado;
    string cidade;
    string curso;
} Aluno;

bool comparaAlunos(Aluno*, Aluno*);

#endif