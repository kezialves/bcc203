#include "fitas.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

void iniciaBloco(Bloco *bloco){
    bloco->alunos = (Aluno*) malloc(TAMANHO_INICIAL_BLOCO * sizeof(Aluno));
}

void iniciaFitas(Fitas *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {

        char nomeFita[30];

        strcpy(nomeFita, NOME_FITAS);

        if(i < 10)
            nomeFita[POSICAO_ALGARISMO_2] = i + 48;
        
        else {
            nomeFita[POSICAO_ALGARISMO_1] = 49;
            nomeFita[POSICAO_ALGARISMO_2] = (i % 10) + 48;
        }

        cout << "FITA " << nomeFita << "\n";

        if((fitas[i]->arquivo = fopen(nomeFita, "wb+")) == NULL)
            cout << "Erro ao abrir o arquivo binário da fita " << nomeFita << ".\n";
    }
}

void adicionaBloco(Fita *fita, Bloco *bloco){
    
    fwrite(&bloco->numeroAlunos, sizeof(int), 1, fita->arquivo);
    fwrite(bloco->alunos, sizeof(Aluno), bloco->numeroAlunos, fita->arquivo);
    fita->numeroBlocos += 1;
}
