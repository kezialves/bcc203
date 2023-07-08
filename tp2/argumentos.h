#ifndef _ARGUMENTOS_H_
#define _ARGUMENTOS_H_

#include <iostream>

using namespace std;

// Estrutura de dados dos argumentos passados em tempo de compilação
typedef struct {
    int metodoOrdenacao; // posição 0
    int quantidadeAlunos; // posição 1
    int tipoOrdenacao; // posição 2
    bool p; // se ativado, imprime o processo
} Argumentos;

#endif