#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include "struct.h"

#define TAM_PAGINA_NO 50

// Estrutura de dados dos nós
typedef struct {
    Registro registro;
    int esquerda, direita;
} No;

// Estrutura de dados das páginas
typedef No PaginaNo[TAM_PAGINA_NO];

// Realiza a pesquisa em Árvore Binária
bool pesquisaBinaria(char*, int, int, Registro*, Performance*);

bool pesquisaBinariaV2(char*, Registro*, Argumentos, Performance*);

// Cria uma Árvore Binária em arquivo binário a partir de outro binário sequencial
bool fazArvoreBinaria(char*, char*, int, Performance*);

// Insere um registro em forma de nó no arquivo da Árvore Binária
bool insereArvoreBinaria(char*,  Registro, Performance*);

// Imprime todos os nós da Árvore Binária
void imprimeArvoreBinaria(char* nomeArvoreBinaria);

#endif