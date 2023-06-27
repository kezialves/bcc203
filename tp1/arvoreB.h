#ifndef ARVORE_B_H
#define ARVORE_B_H

#include "struct.h"

#define ORDEM 2 // ordem da Árvore B

// Estrutura de dados dos apontadores
typedef struct PaginaB *Apontador;

// Estrutura de dados das páginas
typedef struct PaginaB {
    short itensInseridos; // quantidade de posições preenchidas
    Registro registros[2 * ORDEM];
    Apontador apontadores[2 * ORDEM + 1];
} PaginaB;

// Realiza a pesquisa em Árvore B
bool pesquisaB(Argumentos, Registro*, Apontador, Performance*);

// Cria uma Árvore B em arquivo binário a partir de outro binário sequencial
bool fazArvoreB(char*, int, Apontador*, Performance*);

// Inicia a Árvore B nula
void iniciaArvoreB(Apontador);

// Parte iterativa de inserção em Árvore B
bool insereArvoreB(Registro, Apontador*, Performance*);

// Parte recursiva de inserção em Árvore B
bool insereRecursivo(Registro, Apontador, bool*, Registro*, Apontador*, Performance*);

// Insere na página da Árvore B
bool insereNaPagina(Apontador, Registro, Apontador, Performance*);

#endif