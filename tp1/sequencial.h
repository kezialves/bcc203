#ifndef SEQUENCIAL_H
#define SEQUENCIAL_H

#include "struct.h"

// Realiza a busca sequencial
bool pesquisaSequencial(Argumentos, char*, Registro*, Performance*);

// Cria a tabela para a paginação dos registros
int *fazTabela(char*, int, Performance*);

#endif