#include <stdlib.h>
#include <string.h>

#include "fitas.h"
#include "desempenho.h"

using namespace std;

// void iniciaBloco(Bloco *bloco){
//     bloco->alunos = (Aluno*) malloc(TAMANHO_INICIAL_BLOCO * sizeof(Aluno));
// }

void iniciaFitas(Fita *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {

        char nomeFita[30];

        strcpy(nomeFita, NOME_FITAS); // copia o nome padrão

        // Seta o número da fita no nome
        nomeFita[POSICAO_ALGARISMO_1] = (i / 10) + 48; // primeiro dígito
        nomeFita[POSICAO_ALGARISMO_2] = (i % 10) + 48; // segundo dígito
        
        fitas[i].numeroBlocos = 0;

        // cout << "FITA " << nomeFita << "\n";

        // Verifica se foi possível abrir o arquivo binário da fita
        if((fitas[i].arquivo = fopen(nomeFita, "wb+")) == NULL)
            cout << "Erro ao abrir o arquivo binário da fita " << nomeFita << ".\n";
    }
}

void adicionaBloco(Fita *fita, Bloco *bloco, Desempenho *desempenhoCriacao){

    // cout << "------------------\n";

    // for(int i = 0; i < bloco->numeroAlunos; i++){
    //     cout << bloco->alunos[i].nota << endl;
    // }
    // cout << "------------------\n";
    
    // cout << "numero blocos: " << fita->numeroBlocos << endl;

    // Escreve o núemro de alunos
    fwrite(&bloco->numeroAlunos, sizeof(int), 1, fita->arquivo);
    desempenhoCriacao->transferenciasEscrita++;

    // Escreve os alunos
    fwrite(bloco->alunos, sizeof(Aluno), bloco->numeroAlunos, fita->arquivo);
    desempenhoCriacao->transferenciasEscrita++;
    
    // Aumenta o número de blocos na fita
    fita->numeroBlocos = fita->numeroBlocos + 1;
}

void adicionaAluno(Fita fita, Aluno *aluno) {
    fwrite(aluno, sizeof(Aluno), 1, fita.arquivo);
}

int maxBlocos(Fita *fitas, bool fitaIntercalada) {

    int maxBlocos = -1, inicioFitas = 0;
    
    if(fitaIntercalada)
        inicioFitas = NUMERO_FITAS / 2;
    
    for(int i = inicioFitas; i < inicioFitas + (NUMERO_FITAS / 2); i++) {
        if(fitas[i].numeroBlocos > maxBlocos)
            maxBlocos = fitas[i].numeroBlocos;
    }

    return maxBlocos;
}

void flushFitas(Fita *fitas) {

    for (int i = 0; i < NUMERO_FITAS; i++) {
        fflush(fitas[i].arquivo);
    }
}

void reiniciaPonteirosFitas(Fita *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {
        rewind(fitas[i].arquivo);
    }
}

void apagaFitasSaida(Fita *fitas, bool fitasAlternada) {

    for(int i = 0; i < NUMERO_FITAS / 2; i++) {

        // Se as fitas foram alternadas, as de saída são as f primeiras
        if(fitasAlternada) {
            fitas[i].arquivo = freopen(NULL, "wb+", fitas[i].arquivo);
            fitas[i].numeroBlocos = 0;
        }
        
        // Senão, são as f últimas
        else {
            fitas[i + 20].arquivo = freopen(NULL, "wb+", fitas[i + 20].arquivo);
            fitas[i + 20].numeroBlocos = 0;
        }
    }
}

void fechaFitas(Fita *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {
        fclose(fitas[i].arquivo);
    }
}

// Impressão --------------------------------------------------

void imprimeFita(Fita *fita) {

    cout << "numero de blocos na fita: " << fita->numeroBlocos << endl; 

    if(fita->numeroBlocos == 0)
        return;

    rewind(fita->arquivo);

    int numeroAlunos = 0;
    Aluno aluno;

    int bloco = 1;

    while(fread(&numeroAlunos, sizeof(int), 1, fita->arquivo)) {
        
        cout << "\tBLOCO: " << bloco << endl << endl;

        for(int i = 0; i < numeroAlunos; i++) {
            fread(&aluno, sizeof(Aluno), 1, fita->arquivo);
            cout << "\t\tInscrição: " << aluno.numeroInscricao << "\tNota: " << aluno.nota << endl;
        }

        cout << "----------------------------------------\n";
        bloco++;
    }
}

void imprimeFitas(Fita *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {
        cout << "Fita " << i << endl;
        imprimeFita(&fitas[i]);
        cout << "-------------------------------------------\n";
    }
}

// Auxiliar --------------------------------------------------

void converteFitaTexto(Fita fita) {

    int numeroAlunos;
    Aluno aluno;

    FILE *arqT;
    arqT = fopen("testeFita0.txt", "w");
    
    while(fread(&numeroAlunos, sizeof(int), 1, fita.arquivo)) {

        fprintf(arqT, "%d\n", numeroAlunos);

        for(int i = 0; i < numeroAlunos; i++) {
            fread(&aluno, sizeof(Aluno), 1 ,fita.arquivo);
            fprintf(arqT, "%08ld %04.1f %s %-25s %40s\n", aluno.numeroInscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
        }
    }

    fflush(arqT);
    fclose(arqT);
}