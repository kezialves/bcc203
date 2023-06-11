#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "converteBin.h"
#include "struct.h"

using namespace std;

bool textoParaBinario(char *nomeArquivoTexto) {
    
    // Pega o nome do arquivo binário trocando a extensão do arquivo texto
    char *nomeArquivoBinario = trocaExtensao(nomeArquivoTexto);
    
    FILE *arquivoTexto, *arquivoBinario;
    Registro registro;
    
    // Verifica se foi possível abrir o arquivo texto
    // Caso contrário, retorna falso
    if((arquivoTexto = fopen(nomeArquivoTexto,"r")) == NULL) {
        cout << "Erro na abertura do arquivo texto.\n";
        return false;
    }

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "wb")) == NULL) {
        cout << "Erro na criação do arquivo binário.\n";
        return false;
    }

    // Lê os dados do arquivo texto e os escreve no arquivo binário
    while(fscanf(arquivoTexto,"%d", &registro.chave) != EOF) {

        fscanf(arquivoTexto, "%ld %[A-Z a-z0-9]", &registro.dado1, registro.dado2);
        fgetc(arquivoTexto);
        fscanf(arquivoTexto, "%[A-Z a-z0-9]", registro.dado3);

        //cout << registro.chave << " " << registro.dado1 << " " << registro.dado2 << " " << registro.dado3 << "\n";
        fwrite(&registro, sizeof(registro), 1, arquivoBinario);
    }

    free(nomeArquivoBinario);
    fclose(arquivoTexto);
    fclose(arquivoBinario);

    return true;
}

char *trocaExtensao(char *nomeArquivoTexto) {

    // Aloca uma struct para armazenar o nome do arquivo binário
    int tamanhoNome = strlen(nomeArquivoTexto);
    char *nomeArquivoBinario = (char*) malloc((tamanhoNome + 1) * sizeof(char));

    // Copia o nome do arquivo texto no binário e troca o "txt" por "bin"
    strcpy(nomeArquivoBinario, nomeArquivoTexto);
    nomeArquivoBinario[tamanhoNome - 3] = 'b';
    nomeArquivoBinario[tamanhoNome - 2] = 'i';
    nomeArquivoBinario[tamanhoNome - 1] = 'n';

    return nomeArquivoBinario;
}