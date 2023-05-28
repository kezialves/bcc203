#ifndef GERAR_REGISTROS
#define GERAR_REGISTROS

#include <iostream>
using namespace std;

#define numeroRegistros 1000000

#define ORDENACAO 1
// 1 - CRESCENTE
// 2 - DECRESCENTE
// 3 - ALEATÓRIA

// Gera registros aleatórios em um arquivo texto para teste
void criaArquivoTexto(string nomeArquivoTexto);
char RandomStr(const char* array, int arraySize);

#endif