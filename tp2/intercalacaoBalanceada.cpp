#include <stdio.h>
#include "aluno.h"
#include "intercalacaoBalanceada.h"
#include "merge.h"

bool ordenaIntercalacaoBalanceada(Argumentos *argumentos, char *nomeArquivoBinario, Desempenho *desempenho){

    cout << "ENTROU NO ORDENA\n";

    Fitas fitas;

    cout << "INICIANDO FITAS\n";

    iniciaFitas(&fitas);
    
    // Cria os blocos inicialmente ordenados
    switch(argumentos->metodoOrdenacao){

        // Cria os blocos por ordenação interna através do mergeSort
        case 1:
            cout << "CRIANDO BLOCOS\n";
            criaBlocosOrdenacaoInterna(&fitas, desempenho, nomeArquivoBinario);
            break;
        
        // Cria os blocos através da seleção por substituição
        case 2:
            //criaBlocosSubstituicao(&fitas, desempenho, nomeArquivoBinario);
            break;
    }

    cout << "SAIU DO ORDENA\n";

    return true;
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
        
        if(blocoLido.numeroAlunos > 0) {
            adicionaBloco(fitas[numeroFitaAtual % 20], &blocoLido);
            numeroFitaAtual++;
        }

    } while(blocoLido.numeroAlunos != 0);
}