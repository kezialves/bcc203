#include <iostream>
#include <stdio.h>
#include <string.h>

#include "struct.h"
#include "gerarRegistros.h"
#include "converteBin.h"
#include "sequencial.h"
#include "arvoreBinaria.h"

using namespace std;

bool converteArquivo();

int main(int argc, char *argv[]) {

    // criaArquivoTexto("registros_crescente.txt");
    // converteArquivo();
    char nome[50] = "sequencialAleatorio.bin";
    char nomeArvoreBinaria[50] = "arvoreBinaria.bin";

    // imprimeRegistrosBinario(nome);

    // Informa erro quando o número de argumento é inválido
    if(argc < 5 || argc > 6) {
        cout << "Número de argumentos inválidos! São 5 ou 6.\n"
            << "Linha de execução: ./pesquisa <método> <quantidade> <situação> <chave> [-P]\n"
            << "\tExemplo 1 de execução: ./pesquisa 1 100 3 32\n"
            << "\tExemplo 2 de execução: ./pesquisa 2 100000 1 1845 [-P]\n";
        return 0;
    }
    
    // Coloca os argumentos num TAD de Argumentos
    Argumentos argumentos;
    argumentos.metodoPesquisa = atoi(argv[1]);
    argumentos.quantidadeRegistros = atoi(argv[2]);
    argumentos.tipoOrdenacao = atoi(argv[3]);
    argumentos.chave = atoi(argv[4]);

    // Verifica se a impressão dos registros foi requerida
    if(argc == 6 && !strcmp("-P", argv[5]))
        argumentos.p = true;

    else
        argumentos.p = false;

    // Informa erro quando o número do método é inválido
    if(argumentos.metodoPesquisa < 1 || argumentos.metodoPesquisa > 4) {
        cout << "Valor inválido. Os métodos são:\n"
            << "1 - Acesso sequencial indexado;\n"
            << "2 - Árvore Binária de pesquisa adequada à memória externa;\n"
            << "3 - Árvore B;\n"
            << "4 - Árvore B*.\n";
        return 0;
    }
    
    // Informa erro quando o número de registros é inválido
    else if(argumentos.quantidadeRegistros < 100 || argumentos.quantidadeRegistros > 1000000) {
        cout << "Quantidade de registros inválida.\n"
            << "Deve estar entre 100 e 1000000";
        return 0;
    }
    
    // Informa erro quando o número da ordenação é inválido
    else if(argumentos.tipoOrdenacao < 1 || argumentos.tipoOrdenacao > 4) {
        cout << "Situação inválida. A situação do aquivo pode ser:\n"
            << "1 - Arquivo ordenado ascendentemente;\n"
            << "2 - Arquivo ordenado descendentemente;\n"
            << "3 - Arquivo desordenado aleatoriamente.\n";
        return 0;
    }

    Registro registro;

    // Chama o método de pesquisa requerido
    switch(argumentos.metodoPesquisa) {
        
        case 1:

            if(pesquisaSequencial(argumentos, nome, &registro)) {
                cout << "Registro encontrado!" << endl;
            }

            else {
                cout << "Registro não encontrado." << endl;
            }

            break;

        case 2:
            fazArvoreBinaria(nome, nomeArvoreBinaria, argumentos.quantidadeRegistros);
            // imprimeArvoreBinaria(nomeArvoreBinaria);
            
            if(pesquisaBinaria(nomeArvoreBinaria, argumentos.quantidadeRegistros, argumentos.chave, &registro)) {
                cout << "Registro encontrado!" << endl;
            }
            
            else {
                cout << "Registro não encontrado." << endl;
            }

            break;

        // case 3: 
            
        //     break;

        // case 4:

        //     break;
    } 
    
    return 0;
}

bool converteArquivo() {

    string nomeArquivoTexto = "sequencialAleatorio.txt";
    string nomeArquivoBinario = "sequencialAleatorio.bin";

    char arr[nomeArquivoTexto.length() + 1];
    strcpy(arr, nomeArquivoTexto.c_str()); // c_str converte string para const char * e o strcpy faz a cópia para um char*;

    // Tenta converter o arquivo texto para binário e retorna erro caso não consiga
    if(!textoParaBinario(arr)) {
        cout << "Erro na conversão de texto para binário.\n";
        return false;
    }

    strcpy(arr, nomeArquivoBinario.c_str());

    // imprimeRegistrosBinario(arr);

    return true;
}