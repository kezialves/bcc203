#include <iostream>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <stdlib.h>
#include <time.h>

#include "struct.h"
#include "gerarRegistros.h"
#include "converteBin.h"
#include "sequencial.h"
#include "arvoreBinaria.h"
#include "arvoreB.h"
#include "arvoreBest.h"

// #define ITENS_PESQUISADOS 1000000

using namespace std;
using namespace std::chrono;

bool converteArquivo();

int main(int argc, char *argv[]) {

    // criaArquivoTexto("registrosDecrescentes.txt");
    // converteArquivo();

    srand(time(NULL));

    char nome[50] = "registrosCrescentes.txt";
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

    switch(argumentos.tipoOrdenacao){
        case 1:
            strcpy(nome,"registrosCrescentes.bin");
            break;

        case 2:
            strcpy(nome,"registrosDecrescentes.bin");
            break;

        case 3:
            strcpy(nome,"registrosAleatorios.bin");
            break;
    }

    Registro registro;
    Apontador arvoreB = NULL;
    ApontadorBest arvoreBest= NULL;

    // Cria e inicializa as variáveis de controle
    Performance performance;
    iniciaPerformance(&performance);

    // Chama o método de pesquisa requerido
    switch(argumentos.metodoPesquisa) {
        
        case 1:

            // pesquisaSequencial(argumentos, nome, &registro, &performance);

            if(pesquisaSequencial(argumentos, nome, &registro, &performance)) {
                cout << "Registro encontrado!" << endl;
            }

            else {
                cout << "Registro não encontrado." << endl;
            }

            break;

        case 2:

            if(fazArvoreBinaria(nome, nomeArvoreBinaria, argumentos.quantidadeRegistros, &performance)) {

                if(pesquisaBinariaV2(nomeArvoreBinaria, &registro, argumentos, &performance)) {
                    cout << "Registro encontrado!" << endl;
                }

                else {
                    cout << "Registro não encontrado." << endl;
                }
            }
            
            else{
                cout << "Falha na criação da árvore binária em memória externa." << endl;
                return 0;
            }

            // imprimeArvoreBinaria(nomeArvoreBinaria);

            // pesquisaBinariaV2(nomeArvoreBinaria, &registro, argumentos, &performance);
            break;

        case 3:

            if(fazArvoreB(nome, argumentos.quantidadeRegistros, &arvoreB, &performance)) {

                cout << "[Iniciando pesquisa na árvore B...]";
                auto start = high_resolution_clock::now();

                if(pesquisaB(argumentos, &registro, arvoreB, &performance)) {
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    cout << " - Pesquisa realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
                    cout << "Registro encontrado!" << endl;
                }

                else {
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    cout << " - Pesquisa realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
                    cout << "Registro não encontrado." << endl;
                }
            }

             else{
                cout << "Falha na criação da árvore B." << endl;
                return 0;
            }

            // pesquisaB(argumentos, &registro, arvoreB, &performance);

            break;

        case 4:

            if(fazArvoreBest(nome, argumentos.quantidadeRegistros, &arvoreBest, &performance)) {

                cout << "[Iniciando pesquisa na árvore B*...]";
                auto start = high_resolution_clock::now();

                if(pesquisaBest(argumentos, &registro, &arvoreBest, &performance)) {
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    cout << " - Pesquisa realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
                    cout << "Registro encontrado!" << endl;
                }
                
                else {
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    cout << " - Pesquisa realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
                    cout << "Registro não encontrado." << endl;
                }
            }

            else{
                cout << "Falha na criação da árvore B*." << endl;
                return 0;
            }
            
            // pesquisaBest(argumentos, &registro, &arvoreBest, &performance);

            break;
    } 

 
    cout << "Quantidade de comparações: " << performance.comparacoes << endl;
    cout << "Quantidade de transferências: " << performance.transferencias << endl;

    return 0;
}

bool converteArquivo() {

    string nomeArquivoTexto = "registrosDecrescentes.txt";
    string nomeArquivoBinario = "registrosDecrescentes.bin";

    char arr[nomeArquivoTexto.length() + 1];
    strcpy(arr, nomeArquivoTexto.c_str()); // c_str converte string para const char * e o strcpy faz a cópia para um char*

    // Tenta converter o arquivo texto para binário e retorna erro caso não consiga
    if(!textoParaBinario(arr)) {
        cout << "Erro na conversão de texto para binário.\n";
        return false;
    }

    strcpy(arr, nomeArquivoBinario.c_str());

    // imprimeRegistrosBinario(arr);

    return true;
}