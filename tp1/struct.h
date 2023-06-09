#ifndef STRUCT_H
#define STRUCT_H

#define TAM_DADO2 100
#define TAM_DADO3 300
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

// Estrutura de dados dos argumentos passados em tempo de compilação
typedef struct {
    int metodoPesquisa; // posição 0
    int quantidadeRegistros; // posição 1
    int tipoOrdenacao; // posição 2
    int chave; // posição 3
    bool p; // se ativado, imprime o processo
} Argumentos;

// Estrutra de dados das variáveis de controle de performance
typedef struct {
    int transferencias;
    int comparacoes;
} Performance;

// Imprime os registros do arquivo binário
bool imprimeRegistrosBinario(char *nomeArquivoBinario);

// Inicialiaza os valores da struct com 0
void iniciaPerformance(Performance*);

#endif