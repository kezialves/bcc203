#include <iostream>

#include "aluno.h"
#include "desempenho.h"

using namespace std;

// Compara dois alunos e retorna true se a nota do primeiro for maior.
bool comparaAlunos(Aluno *aluno1, Aluno *aluno2, Desempenho *desempenhoCriacao) {
    
    desempenhoCriacao->comparacoes += 1;
    
    if(aluno1->nota <= aluno2->nota)
        return true;
    
    else
        return false;
}