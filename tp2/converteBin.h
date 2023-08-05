#ifndef CONVERTE_BIN_H
#define CONVERTE_BIN_H

#include "argumentos.h"

// Converte um arquivo texto para binário
bool textoParaBinario(char*, char**, Argumentos*);

// Converte um arquivo binário para texto
bool binarioParaTexto(char*, char*, Argumentos*);

// Converte o .txt do nome do arquivo para .bin
char * trocaExtensao(char*);

#endif