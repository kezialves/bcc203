#include <iostream>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "converteBin.h"
#include "gerarRegistros.h"
#include "sequencial.h"

using namespace std;

bool converteArquivo();

int main(int argc, char *argv[]) {

    criaArquivoTexto("registros_crescente.txt");
    converteArquivo();
    char nome[50] = "registros_crescente.bin";

    fazTabela(nome, 100);

    /* if(argc < 5 || argc > 6) {

        cout << "Número de argumentos inválidos! São 5 ou 6.\n";
        cout << "Linha de execução: ./pesquisa <método> <quantidade> <situação> <chave> [-P]\n";
        cout << "\tExemplo 1 de execução: ./pesquisa 1 100 3 32\n";
        cout << "\tExemplo 2 de execução: ./pesquisa 2 100000 1 1845 [-P]\n";

        return 0;
    }
    
    Argumentos argumentos;
    argumentos.metodoPesquisa = atoi(argv[1]);
    argumentos.quantidadeRegistros = atoi(argv[2]);
    argumentos.tipoOrdenacao = atoi(argv[3]);
    argumentos.chave = atoi(argv[4]);

    if(argumentos.metodoPesquisa < 1 || argumentos.metodoPesquisa > 4) {

        cout << "Valor inválido. Os métodos são:\n" <<
            "1 - Acesso sequencial indexado;\n" <<
            "2 - Árvore binária de pesquisa adequada à memória externa;\n" <<
            "3 - Árvore B;\n"
            "4 - Árvore B*.\n";

        return 0;
    }
    
    else if(argumentos.quantidadeRegistros < 100 || argumentos.quantidadeRegistros > 1000000) {
        
        cout << "Quantidade de registros inválida.\n" <<
            "Deve estar entre 100 e 1000000";
            
        return 0;
    }
    
    else if(argumentos.tipoOrdenacao < 1 || argumentos.tipoOrdenacao > 4) {

        cout << "Situação inválida. A situação do aquivo pode ser:\n" <<
        "1 - Arquivo ordenado ascendentemente;\n" <<
        "2 - Arquivo ordenado descendentemente;\n" <<
        "3 - Arquivo desordenado aleatoriamente.\n";

        return 0;
    } */

   /*  switch(argumentos.metodoPesquisa) {
        
        case 1:
            cout << "O panda é uma das raças de panda mais fofas do planeta terra";
            break;

        // case 2:

        //     break;

        // case 3: 
            
        //     break;

        // case 4:

        //     break;
    } */
    
    return 0;
}

bool converteArquivo() {

    string nomeArquivoTexto = "registros_crescente.txt";
    string nomeArquivoBinario = "registros_crescente.bin";

    char arr[nomeArquivoTexto.length() + 1];
    strcpy(arr, nomeArquivoTexto.c_str()); // c_str converte string para const char * e o strcpy faz a cópia para um char*;

    if(!textoParaBinario(arr)) {
        cout << "Erro na conversão de texto para binário.\n";
        return false;
    }

    strcpy(arr, nomeArquivoBinario.c_str());

    // imprimeRegistrosBinario(arr);

    return true;
}