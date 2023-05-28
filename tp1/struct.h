#ifndef STRUCT_H
#define STRUCT_H

#define TAM_DADO2 1000
#define TAM_DADO3 5000
#define ITENS_PAGINA 50

// Estrutura de dados dos registros
typedef struct {
    int chave;
    long dado1;
    char dado2[TAM_DADO2];
    char dado3[TAM_DADO3];
} Registro;

// Vetor responsável pela paginação do arquivo
typedef Registro Pagina[ITENS_PAGINA];

bool imprimeRegistrosBinario(char *nomeArquivoBinario);

#endif