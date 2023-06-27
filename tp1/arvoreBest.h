#ifndef ARVORE_BEST_H
#define ARVORE_BEST_H

#include "struct.h"

// Ordem da Árvore B*
#define M 2
#define MM M*2
#define MM2 MM

// Definição do nível da página
typedef enum {Interna, Externa} NivelPagina;

// Estrutura de dados dos apontadores
typedef struct PaginaBest *ApontadorBest;

// Estrutura de dados das páginas
typedef struct PaginaBest {
    NivelPagina tipoPagina;

    // Usamos union para evitar criar dois tipos de páginas e apontadores
    union {
        // Página interna, nó, funciona como índice
        struct {
            int quantidadeChaves; //ni
            int chaves[MM]; //ri
            ApontadorBest apontadores[MM + 1]; //pi
        } U0; // interna
        
        // Página externa, folha, armazena os registros propriamente ditos
        struct {
            int quantidadeRegistros; //ne
            Registro registros[MM2]; //re
        } U1; // externa
    } UU; // nível da página
} PaginaBest;

// Realiza a pesquisa em Árvore B*
bool pesquisaBest(Argumentos, Registro*, ApontadorBest*, Performance*);

// Cria uma Árvore B* em arquivo binário a partir de outro binário sequencial
bool fazArvoreBest(char*, int, ApontadorBest*, Performance*);

// Inicia a Árvore B* com uma página folha vazia
void iniciaArvoreBest(ApontadorBest*);

// Parte iterativa de inserção em Árvore B*
bool insereArvoreBest(Registro, ApontadorBest*, Performance*);

// Parte recursiva de inserção em Árvore B*
bool insereRecursivo(Registro, ApontadorBest, bool*, Registro*, ApontadorBest*, Performance*);

// Insere na página interna, uma página nó, da Árvore B*
bool insereNaPaginaInterna(ApontadorBest, int, ApontadorBest, Performance*);

// Insere na página externa, uma página folha, da Árvore B*
bool insereNaPaginaExterna(ApontadorBest, Registro, Performance*);

#endif