#include <iostream>
#include <stdio.h>

#include "struct.h"

using namespace std;

bool imprimeRegistrosBinario(char *nomeArquivoBinario) {
    
    FILE *arquivoBinario;
    Registro registro;

    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    while(fread(&registro, sizeof(Registro), 1, arquivoBinario)) {
        cout << registro.chave << " " << registro.dado1 << " " << registro.dado2 << " " << registro.dado3 << "\n";
    }

    fclose(arquivoBinario);
    return true;
}