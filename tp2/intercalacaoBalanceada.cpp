#include <stdio.h>
#include "aluno.h"
#include "intercalacaoBalanceada.h"
#include "merge.h"

bool ordenaIntercalacaoBalanceada(Argumentos *argumentos, char *nomeArquivoBinario, Desempenho *desempenho){

    Fita fitas[NUMERO_FITAS];

    iniciaFitas(fitas);
    
    // Cria os blocos inicialmente ordenados
    switch(argumentos->metodoOrdenacao){

        // Cria os blocos por ordenação interna através do mergeSort
        case 1:
            criaBlocosOrdenacaoInterna(fitas, desempenho, nomeArquivoBinario);
            break;
        
        // Cria os blocos através da seleção por substituição
        case 2:
            //criaBlocosSubstituicao(&fitas, desempenho, nomeArquivoBinario);
            break;
    }

    imprimeFitas(fitas);

    return true;
}

void criaBlocosOrdenacaoInterna(Fita *fitas, Desempenho *desempenho, char *nomeArquivoBinario){

    FILE *arquivoBinario;

    if((arquivoBinario = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }
    
    Bloco blocoLido;
    // iniciaBloco(&blocoLido);

    int numeroFitaAtual = 0;

    do {

        blocoLido.numeroAlunos = fread(blocoLido.alunos, sizeof(Aluno), TAMANHO_INICIAL_BLOCO, arquivoBinario);
        // cout << "Numero Alunos: " << blocoLido.numeroAlunos << endl;

        mergeSort(blocoLido.alunos, 0, blocoLido.numeroAlunos - 1);
        // cout << "------------------\n";

        // for(int i = 0; i < blocoLido.numeroAlunos;i++){
        //     cout << blocoLido.alunos[i].nota << endl;
        // }
        // cout << "------------------\n";

        // cout << "Numero Fita atual: " << numeroFitaAtual << endl;

        if(blocoLido.numeroAlunos > 0) {
            adicionaBloco(&fitas[numeroFitaAtual % 20], &blocoLido);
            numeroFitaAtual++;
        }

    } while(blocoLido.numeroAlunos != 0);

}

bool intercala(Fita* fitas){

    int vetorControle[20];
    Bloco alunos;

    for(int i = 0; i < NUMERO_FITAS; i++){
        if(fitas[i].numeroBlocos == 0)
            break;
        rewind(fitas->arquivo);
        fread(&(vetorControle[i]), sizeof(int), 1, fitas[i].arquivo);
    }

    

}

int menorElemento(Bloco *memoriaPrincipal, int *vetorControle){

    int indiceMenor;

    for(int i = 0; i < NUMERO_FITAS; i++){
        if(vetorControle[i]){
            indiceMenor = i;
            break;
        }
    }

    for(int i = indiceMenor + 1; i < TAMANHO_MEMORIA_INTERNA; i++){
        if (memoriaPrincipal->alunos[i].nota < memoriaPrincipal->alunos[indiceMenor].nota &&
            vetorControle[i] > 0){
            indiceMenor = i;
        }
    }

    return indiceMenor;

}
