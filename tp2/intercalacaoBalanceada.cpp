#include <stdio.h>
#include "aluno.h"
#include "intercalacaoBalanceada.h"
#include "merge.h"

bool ordenaIntercalacaoBalanceada(Argumentos *argumentos, char *nomeArquivoBinario, Desempenho *desempenho){

    Fitas fitas;
    
    // Cria os blocos inicialmente ordenados
    switch(argumentos->metodoOrdenacao){

        // Cria os blocos por ordenação interna através do mergeSort
        case 1:
            criaBlocosOrdenacaoInterna(&fitas, desempenho, nomeArquivoBinario);
            break;
        
        // Cria os blocos através da seleção por substituição
        case 2:
            criaBlocosSubstituicao(&fitas, desempenho, nomeArquivoBinario);
            break;
    }

}

void criaBlocosOrdenacaoInterna(Fitas *fitas, Desempenho *desempenho, char *nomeArquivoBinario){

    FILE *arquivoBinario;

    if((arquivoBinario = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }
    
    Bloco blocoLido;
    iniciaBloco(&blocoLido);
    int numeroFitaAtual = 0;

    do {

        blocoLido.numeroAlunos = fread(blocoLido.alunos, sizeof(Aluno), TAMANHO_INICIAL_BLOCO, arquivoBinario);
        mergeSort(blocoLido.alunos, 0, blocoLido.numeroAlunos);
        
        if(blocoLido.numeroAlunos > 0){
            adicionaBloco(fitas, &blocoLido);
        }

    } while(blocoLido.numeroAlunos != 0);

}