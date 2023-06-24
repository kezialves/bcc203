#ifndef ARVORE_BINARIA
#define ARVORE_BINARIA

#include "struct.h"

#define TAM_PAGINA_NO 50

// Estrutura de dados dos nós
typedef struct {
    Registro registro;
    int esquerda, direita;
} No;

//
typedef No PaginaNo[TAM_PAGINA_NO];

// Direção de caminhamento na árvore binária
// enum Direcao {Esquerda = 0, Direita = 1};

// Realiza a pesquisa em árvore binária
bool pesquisaBinaria(char*, int, int, Registro*);

// Cria uma árvore binária em arquivo binário a partir de outro binário sequencial
bool fazArvoreBinaria(char*, char*, int);

// Insere um registro em forma de nó no arquivo da árvore binária
bool insereArvoreBinaria(char*,  Registro);

// Imprime todos os nós da árvore binária
void imprimeArvoreBinaria(char* nomeArvoreBinaria);

#endif