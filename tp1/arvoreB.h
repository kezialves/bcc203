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
bool pesquisaB(int, Registro*, Apontador);

// Cria uma Árvore B em arquivo binário a partir de outro binário sequencial
bool fazArvoreB(char*, int, Apontador*);

// Inicia a Árvore B nula
void iniciaArvoreB (Apontador);

// 
bool insereArvoreB(Registro, Apontador*);

//
bool insereRecursivo(Registro, Apontador, bool*, Registro*, Apontador*);

//
bool insereNaPagina(Apontador, Registro, Apontador);

// Imprime todos os nós da Árvore B
void imprimeArvoreB(Apontador);

#endif