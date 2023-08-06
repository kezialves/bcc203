#ifndef _BINARIOS_H_
#define _BINARIOS_H_

#include "argumentos.h"

// Gera o arquivo binário de acordo com a situação de ordenação
void geraBinario(Argumentos*, char**);

// Imprime um arquivo binário
void imprimeRegistrosBinario(char*);

// Converte um arquivo texto para binário
bool textoParaBinario(char*, char**, Argumentos*);

// Converte um arquivo binário para texto
bool binarioParaTexto(char*, char*, Argumentos*);

// Converte o .txt do nome do arquivo para .bin
char * trocaExtensao(char*);

#endif