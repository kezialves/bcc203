#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include "gerarRegistros.h"
#include "struct.h"

using namespace std;

void criaArquivoTexto(string nomeArquivoTexto) {

    srand((unsigned) time(NULL));

    Registro *arrayRegistros = (Registro*) malloc(numeroRegistros * sizeof(Registro));

    static const char AlphaNumeric[] = "0123456789"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ "
                                "abcdefghijklmnopqrstuvwxyz";

    int charArrayLen = sizeof(AlphaNumeric) - 1;

    ofstream arquivoTexto;
    arquivoTexto.open(nomeArquivoTexto);

    int *randomIntArray;

    if(ORDENACAO == 3){
        randomIntArray = (int*) malloc (numeroRegistros * sizeof(int));

        for(int i = 0 ; i < numeroRegistros; i++)
            randomIntArray[i] = i + 1;

        random_shuffle(&randomIntArray[0], &randomIntArray[numeroRegistros]);
    }

    for(int i = 0; i < numeroRegistros; i++) {

        switch(ORDENACAO) {

            case 1:
                arrayRegistros[i].chave = i + 1;
                break;
                
            case 2:
                arrayRegistros[i].chave = numeroRegistros - i;
                break;
            
            case 3:
                arrayRegistros[i].chave = randomIntArray[i];
                break;
            
            default:
                break;
        }

        arrayRegistros[i].dado1 = rand();

        int repeticaoDado2 = random() %  (TAM_DADO2 - 1) + 1;
        
        for(int j = 0; j < repeticaoDado2; j++) {
            arrayRegistros[i].dado2[j] = RandomStr(AlphaNumeric, charArrayLen);
        }

        int repeticaoDado3 = random() %  (TAM_DADO3 - 1) + 1;
        
        for(int j = 0; j < repeticaoDado3; j++) {
            arrayRegistros[i].dado3[j] = RandomStr(AlphaNumeric, charArrayLen);
        }
    }

    for(int i = 0; i < numeroRegistros; i++) {
        arquivoTexto << arrayRegistros[i].chave << " " << 
        arrayRegistros[i].dado1 << " " << 
        arrayRegistros[i].dado2 << "|" <<
        arrayRegistros[i].dado3 << "\n";
    }

    if(ORDENACAO == 3)
        free(randomIntArray);
    
    free(arrayRegistros);
    arquivoTexto.close();
}

char RandomStr(const char* array, int arraySize) {
    return array[rand() % arraySize];
}

/*
srand(time(0));

    for (int i = 0; i < 1000000; i++) {

        int chave = rand();
        long int dado1 = rand();
        string dado2;
        string dado3;

        for (int j = 0; j < 1000; j++) {
            dado2 += static_cast<char>(rand() % 26 + 65); // Caracteres aleatórios de A a Z
        }

        for (int j = 0; j < 5000; j++) {
            dado3 += static_cast<char>(rand() % 26 + 65); // Caracteres aleatórios de A a Z
        }
    }
*/