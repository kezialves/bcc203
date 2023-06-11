#include <iostream>
#include "arquivo.h"

using namespace std;

bool abreArquivo(FILE *arquivoBinario, char *nomeArquivoBinario) {

    // printf("Nome do arquivo dentro de abreArquivo: %s\n", nomeArquivoBinario);

    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Não foi possível abrir o arquivo binário\n";
        return false;
    }

    // printf("-----Passou do if\n");
    
    return true;
}