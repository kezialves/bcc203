#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include "gerarRegistros.h"
#include "struct.h"

using namespace std;

void criaArquivoTexto(string nomeArquivoTexto) {

    srand((unsigned) time(NULL));

    Registro *arrayRegistros = (Registro*) malloc(QTD_REGISTROS * sizeof(Registro));

    static const char AlphaNumeric[] = "0123456789"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
                                "abcdefghijklmnopqrstuvwxyz";

    int charArrayLen = sizeof(AlphaNumeric) - 1;

    ofstream arquivoTexto;
    arquivoTexto.open(nomeArquivoTexto);

    int *randomIntArray;

    // Se o arquivo for aleatório,
    // gera um vetor de inteiros aleatórios para as chaves
    if(ORDENACAO == 3) {
        randomIntArray = (int*) malloc (QTD_REGISTROS * sizeof(int));

        for(int i = 0 ; i < QTD_REGISTROS; i++)
            randomIntArray[i] = i + 1;

        random_shuffle(&randomIntArray[0], &randomIntArray[QTD_REGISTROS]);
    }

    // Preenche o arquivo
    for(int i = 0; i < QTD_REGISTROS; i++) {

        // Coloca as chaves de acordo com o tipo de ordenação
        switch(ORDENACAO) {

            // Ordenação ascendente
            case 1:
                arrayRegistros[i].chave = i + 1;
                break;
                
            // Ordenação descendente
            case 2:
                arrayRegistros[i].chave = QTD_REGISTROS - i;
                break;
            
            // Aleatório
            case 3:
                arrayRegistros[i].chave = randomIntArray[i];
                break;
            
            default:
                break;
        }

        // Gera o dado 1, um inteiro
        arrayRegistros[i].dado1 = rand();
        
        // Gera o dado 2, uma string
        for(int j = 0; j < TAM_DADO2 - 1 ; j++) {
            arrayRegistros[i].dado2[j] = RandomStr(AlphaNumeric, charArrayLen);
        }

        // Coloca o \0 no final do dado 2
        arrayRegistros[i].dado2[TAM_DADO2 - 1] = '\0';
        
        // Gera o dado 3, uma string maior do que a anterior
        for(int j = 0; j < TAM_DADO3 - 1; j++) {
            arrayRegistros[i].dado3[j] = RandomStr(AlphaNumeric, charArrayLen);
        }

        // Coloca o \0 no final do dado 3
        arrayRegistros[i].dado3[TAM_DADO3 - 1] = '\0';
    }

    // Insere os registros no arquivo texto,
    for(int i = 0; i < QTD_REGISTROS; i++) {
        arquivoTexto << arrayRegistros[i].chave << " " << // insere a chave e separa com um espaço
        arrayRegistros[i].dado1 << " " << // insere o dado 1 e separa com um espaço
        arrayRegistros[i].dado2 << "|" << // insere o dado 2 e separa com um pipeline, já que a string pode conter espaços
        arrayRegistros[i].dado3 << "\n"; // insere o dado 3 e dá um enter
    }
    
    /* char nomeArquivoBinario[50] = "registros_crescentes.bin";
    FILE *arquivoBinario = fopen(nomeArquivoBinario,"wb");
    fwrite(arrayRegistros, sizeof(Registro), QTD_REGISTROS, arquivoBinario); */

    if(ORDENACAO == 3)
        free(randomIntArray);
    
    free(arrayRegistros);
    arquivoTexto.close();
}

char RandomStr(const char* array, int arraySize) {
    return array[rand() % arraySize];
}