#include <iostream>

#include "desempenho.h"

using namespace std;

void iniciaDesempenho(Desempenho *desempenho) {
    desempenho->transferenciasLeitura = 0;
    desempenho->transferenciasEscrita = 0;
    desempenho->comparacoes = 0;
}