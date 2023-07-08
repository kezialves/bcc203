#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>

#include "argumentos.h"
#include "desempenho.h"
#include "fitas.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {

    Fita fita;

    srand(time(NULL));

    //Cria e inicializa as variáveis de controle
    Desempenho desempenho;
    // iniciaDesempenho(&desempenho);

    // Informa erro quando o número de argumentos é inválido
    if(argc < 4 || argc > 5) {
        cout << "Número de argumentos inválidos! São 4 ou 5.\n"
            << "Linha de execução: ./ordena <método> <quantidade> <situação> [-P]\n"
            << "\tExemplo 1 de execução: ./ordena 1 100 3\n"
            << "\tExemplo 2 de execução: ./ordena 2 100000 1 [-P]\n";
        return 0;
    }
    
    // Coloca os argumentos num TAD de Argumentos
    Argumentos argumentos;
    argumentos.metodoOrdenacao = atoi(argv[1]);
    argumentos.quantidadeAlunos = atoi(argv[2]);
    argumentos.tipoOrdenacao = atoi(argv[3]);

    // Verifica se a impressão dos registros foi requerida
    if(argc == 5 && !strcmp("-P", argv[4]))
        argumentos.p = true;

    else
        argumentos.p = false;

    // Informa erro quando o número do método é inválido
    if(argumentos.metodoOrdenacao < 1 || argumentos.metodoOrdenacao > 3) {
        cout << "Valor inválido. Os métodos são:\n"
            << "1 - Intercalação balanceada de vários caminhos (2f fitas), utilizando o mergesort para ordenação;\n"
            << "2 - Intercalação balanceada de vários caminhos (2f fitas), utilizando seleção por substituição.\n"
            << "3 - Quicksort externo;\n";
        return 0;
    }
    
    // Informa erro quando o número de alunos é inválido
    else if(argumentos.quantidadeAlunos < 100 || argumentos.quantidadeAlunos > 471705) {
        cout << "Quantidade de Alunos inválida.\n"
            << "Deve estar entre 100 e 471.705";
        return 0;
    }
    
    // Informa erro quando o número do tipo de ordenação é inválido
    else if(argumentos.tipoOrdenacao < 1 || argumentos.tipoOrdenacao > 3) {
        cout << "Situação inválida. A situação do aquivo pode ser:\n"
            << "1 - Arquivo ordenado ascendentemente;\n"
            << "2 - Arquivo ordenado descendentemente;\n"
            << "3 - Arquivo desordenado aleatoriamente.\n";
        return 0;
    }

    switch(argumentos.metodoOrdenacao) {
        
        case 1:
            break;

        case 2:

            break;

        case 3:

            break;

        case 4:


            break;
    } 

    return 0;
}