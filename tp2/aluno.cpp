#include <iostream>

#include "aluno.h"

using namespace std;

//Compara dois alunos, e retorna True se a nota do 1 for maior.
bool comparaAlunos(Aluno* aluno1, Aluno* aluno2){
    if (aluno1->nota <= aluno2->nota)
        return true;
    else
        return false;
}