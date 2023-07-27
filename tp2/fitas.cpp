#include "fitas.h"
#include <stdlib.h>

using namespace std;

void iniciaBloco(Bloco *bloco){
    bloco->alunos = (Aluno*) malloc(TAMANHO_INICIAL_BLOCO * sizeof(Aluno));
}

void adicionaBloco(Fitas*, Bloco*){
    

}
