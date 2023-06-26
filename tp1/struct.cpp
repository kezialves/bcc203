#include <iostream>
#include <stdio.h>

#include "struct.h"

using namespace std;

bool imprimeRegistrosBinario(char *nomeArquivoBinario) {
    
    FILE *arquivoBinario;
    Registro registro;

    // Tenta abrir o arquivo binário e retorna erro caso não consiga
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    // Imprime os registros
    while(fread(&registro, sizeof(Registro), 1, arquivoBinario)) {
        cout << registro.chave << " " << registro.dado1 << " " << registro.dado2 << " " << registro.dado3 << "\n";
    }

    fclose(arquivoBinario);
    return true;
}

void iniciaPerformance(Performance *performance) {
    performance->comparacoes = 0;
    performance->transferencias = 0;
}