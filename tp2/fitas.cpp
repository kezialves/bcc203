#include "fitas.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

// void iniciaBloco(Bloco *bloco){
//     bloco->alunos = (Aluno*) malloc(TAMANHO_INICIAL_BLOCO * sizeof(Aluno));
// }

void iniciaFitas(Fita *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {

        char nomeFita[30];

        strcpy(nomeFita, NOME_FITAS);

        nomeFita[POSICAO_ALGARISMO_1] = (i / 10) + 48;
        nomeFita[POSICAO_ALGARISMO_2] = (i % 10) + 48;
        
        fitas[i].numeroBlocos = 0;
        // cout << "FITA " << nomeFita << "\n";

        if((fitas[i].arquivo = fopen(nomeFita, "wb+")) == NULL)
            cout << "Erro ao abrir o arquivo binário da fita " << nomeFita << ".\n";
    }
}

void adicionaBloco(Fita *fita, Bloco *bloco){

    // cout << "------------------\n";

    // for(int i = 0; i < bloco->numeroAlunos; i++){
    //     cout << bloco->alunos[i].nota << endl;
    // }
    // cout << "------------------\n";
    
    // cout << "numero blocos: " << fita->numeroBlocos << endl;
    fwrite(&bloco->numeroAlunos, sizeof(int), 1, fita->arquivo);
    fwrite(bloco->alunos, sizeof(Aluno), bloco->numeroAlunos, fita->arquivo);
    fita->numeroBlocos = fita->numeroBlocos + 1;
}

void imprimeFita(Fita *fita){

    if(fita->numeroBlocos == 0)
        return;

    rewind(fita->arquivo);

    int alunosBloco = 0;
    Aluno aluno;

    for(int j = 0; j < fita->numeroBlocos; j++){

        fread(&alunosBloco, sizeof(int), 1, fita->arquivo);
        cout << "\tBloco " << j << endl;

        do {
               
            fread(&aluno, sizeof(Aluno), 1, fita->arquivo);
            cout << "\t\tInscrição: " << aluno.numeroInscricao << "\tNota: " << aluno.nota << endl;
            alunosBloco--;
            // cout << "Alunos bloco: " << alunosBloco << endl;
        } while(alunosBloco > 0);
    }
}

void imprimeFitas(Fita *fitas){

    for(int i = 0; i < NUMERO_FITAS; i++){
        cout << "Fita " << i << endl; 
        imprimeFita(&fitas[i]);
        cout << "-------------------------------------------\n";
    }
}
