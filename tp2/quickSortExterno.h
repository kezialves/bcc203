#ifndef _QUICK_SORT_EXTERNO_
#define _QUICK_SORT_EXTERNO_

#include "aluno.h"
#include "argumentos.h"
#include "desempenho.h"

#define TAMANHO_AREA 20

// Estrutura de dados da área do QuickSort
typedef struct {
    Aluno alunos[TAMANHO_AREA];
    int numeroAlunos = 0;
} Area;

// Parte iterativa do QuickSort externo
void ordenaQuickSort(char*, Argumentos*, Desempenho*);

// Parte recursiva do QuickSort externo
void quickSortExterno(FILE**, FILE**, FILE**, int, int, Desempenho*);

// Faz uma partição
void particao(FILE**, FILE**, FILE**, Area, int, int, int*, int*, Desempenho*);

// Cria uma área vazia
void fazAreaVazia(Area*);

// Retorna a quantidade de células ocupadas na área
int obterNumCelOcupadas(Area*);

// Realiza uma leitura inferior
void leInferior(FILE**, Aluno*, int*, bool*, Desempenho*);

// Realiza uma leitura superior
void leSuperior(FILE**, Aluno*, int*, bool*, Desempenho*);

// Insere o último aluno lido na área
void inserirArea(Area*, Aluno*, int*, Desempenho*);
void insereItem(Aluno*, Area*, Desempenho*);

// Realiza uma escrita inferior
void escreveMinimo(FILE**, Aluno, int*, Desempenho*);

// Realiza uma escrita superior
void escreveMaximo(FILE**, Aluno, int*, Desempenho*);

// Retira o menor aluno da área
void retiraMinimo(Area*, Aluno*, int*);
void retiraUltimo(Area*, Aluno*);

// Retira o maior aluno da área
void retiraMaximo(Area*, Aluno*, int*);
void retiraPrimeiro(Area*, Aluno*);

#endif