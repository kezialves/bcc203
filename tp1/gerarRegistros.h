#ifndef GERAR_REGISTROS_H
#define GERAR_REGISTROS_H

#include <iostream>
using namespace std;

#define QTD_REGISTROS 100

#define ORDENACAO 1
// 1 - CRESCENTE
// 2 - DECRESCENTE
// 3 - ALEATÓRIA

// Gera registros aleatórios em um arquivo texto para teste
void criaArquivoTexto(string nomeArquivoTexto);

// Gera uma string aleatória
char RandomStr(const char *array, int arraySize);

#endif