#include <iostream>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "converteBin.h"
#include "gerarRegistros.h"

using namespace std;

int main(int argc, char *argv[]) {

    criaArquivoTexto("registros_crescente.txt");

    // if(argc < 5 || argc > 6) {
    //     cout << "Número de argumentos inválidos! São 5 ou 6.\n";
    //     cout << "Linha de execução: ./pesquisa <método> <quantidade> <situação> <chave> [-P]\n";
    //     cout << "\tExemplo 1 de execução: ./pesquisa 1 100 3 32\n";
    //     cout << "\tExemplo 2 de execução: ./pesquisa 2 100000 1 1845 [-P]\n";
    //     return 0;
    // }
    
    // int argumentos[4];

    // argumentos[0] = atoi(argv[1]);
    // argumentos[1] = atoi(argv[2]);
    // argumentos[2] = atoi(argv[3]);
    // argumentos[3] = atoi(argv[4]);

    // if(argumentos[0] < 1 || argumentos[0] > 4) {
        
    //     cout << "Valor inválido. Os métodos são:\n" <<
    //         "1 - Acesso sequencial indexado;\n" <<
    //         "2 - Árvore binária de pesquisa adequada à memória externa;\n" <<
    //         "3 - Árvore B;\n"
    //         "4 - Árvore B*.\n";
    //     return 0;
    // }
    
    // else if(argumentos[1] < 100 || argumentos[1] > 1000000) {
        
    //     cout << "Quantidade de registros inválida.\n" <<
    //         "Deve estar entre 100 e 1000000";
            
    //     return 0;
    // }
    
    // else if(argumentos[2] < 1 || argumentos[2] > 4) {
    //     cout << "Situação inválida. A situação do aquivo pode ser:\n" <<
    //     "1 - Arquivo ordenado ascendentemente;\n" <<
    //     "2 - Arquivo ordenado descendentemente;\n" <<
    //     "3 - Arquivo desordenado aleatoriamente.\n";
    // }

    // switch(argumentos[0]) {
    //     case 1:
    //         cout << "O panda é uma das raças de panda mais fofas do planeta terra";
    //         break;

    //     case 2:

    //         break;

    //     case 3: 
            
    //         break;

    //     case 4:

    //         break;
    // }
    
    return 0;
}



bool converteArquivo() {

    string nomeArquivoTexto = "1.txt";
    string nomeArquivoBinario = "1.bin";

    char arr[nomeArquivoTexto.length() + 1];
    strcpy(arr, nomeArquivoTexto.c_str()); // c_str converte string para const char * e o strcpy faz a cópia para um char*;

    if(!textoParaBinario(arr)) {
        cout << "Erro na conversão de texto para binário.\n";
        return false;
    }

    strcpy(arr, nomeArquivoBinario.c_str());

    imprimeRegistrosBinario(arr);

    return true;
}

