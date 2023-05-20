#include <iostream>
#include "arquivo.h"

using namespace std;

bool abreArquivo(FILE * arquivoBinario, char *nomeArquivoBinario) {

    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {

        cout << "Não foi possível abrir o arquivo binário\n";
        return false;
    }
    
    return true;
}