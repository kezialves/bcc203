#ifndef _DESEMPENHO_H_
#define _DESEMPENHO_H_

#include <chrono>

using namespace std::chrono;

// Estrutra de dados das variáveis de controle de performance
typedef struct {
    int transferenciasLeitura = 0;
    int transferenciasEscrita = 0;
    int comparacoes = 0;
    nanoseconds tempoExecucao;
} Desempenho;

#endif