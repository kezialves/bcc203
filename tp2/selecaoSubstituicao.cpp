#include <stdio.h>
#include <limits>

#include "heap.h"
#include "selecaoSubstituicao.h"

using namespace std;

void criaBlocosSelecaoSubstituicao(Fita *fitas, char *nomeArquivoBinario, Desempenho *desempenhoCriacao) {

    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }

    auto start = high_resolution_clock::now();

    Heap heap;
    int numeroFitaAtual = 0;
    int numeroAlunosFitaAtual = 0;

    // Pega o número de alunos da fita atual
    fwrite(&numeroAlunosFitaAtual, sizeof(int), 1, fitas[numeroFitaAtual].arquivo);
    desempenhoCriacao->transferenciasEscrita += 1;
    fitas[numeroFitaAtual].numeroBlocos++;
    
    //// Marcado quando é menor.
    //bool vetorTags[NUMERO_FITAS / 2] = {false};
    
    int alunoAtual = 0;
    int leu = -1;
    
    // Inicializa todos os alunos como não marcados
    for(int i = 0; i < (NUMERO_FITAS / 2); i++){
        heap.alunos[i].marcado = false;
    }

    // Lê os alunos para o heap
    while((alunoAtual < (NUMERO_FITAS / 2)) && (leu = fread(&heap.alunos[alunoAtual].aluno, sizeof(Aluno), 1, arquivoBinario))) {
        leu = -1;
        desempenhoCriacao->transferenciasLeitura += 1;
        heap.numeroAlunos++;
        alunoAtual++;
    }

    if(leu == 0)
        desempenhoCriacao->transferenciasLeitura += 1; 

    // Constrói o heap com os alunos lidos
    heapConstroi(&heap, desempenhoCriacao);

    while(heap.numeroAlunos > 0){ // enquanto tiver alunos no heap

        // Se todos os alunos forem menores
        if(todosMarcados(&heap)) {

            // Desmarca todos e constrói o heap
            desmarcaTodos(&heap);
            heapConstroi(&heap, desempenhoCriacao);
            
            // Atualiza a fita
            numeroFitaAtual++;
            fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].numeroBlocos += 1;
            numeroAlunosFitaAtual = 0;

            fwrite(&numeroAlunosFitaAtual, sizeof(int), 1, fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo);
            desempenhoCriacao->transferenciasEscrita += 1;
        }

        // Escreve o menor aluno (sempre a posição 0 do heap) na fita e incrementa o número de alunos no bloco
        fwrite(&heap.alunos[0].aluno, sizeof(Aluno), 1, fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo);
        desempenhoCriacao->transferenciasEscrita += 1;
        numeroAlunosFitaAtual++;
        
        // Volta para o número que tem no começo do bloco falando a quantidade de alunos
        fseek(fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo, -(numeroAlunosFitaAtual * sizeof(Aluno)) - sizeof(int), SEEK_END);
        
        // e escreve a quantidade atualizada
        fwrite(&numeroAlunosFitaAtual, sizeof(int), 1, fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo);
        desempenhoCriacao->transferenciasEscrita += 1;
        
        fseek(fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo, 0 , SEEK_END);  

        double comparaNotaMarcacao = heap.alunos[0].aluno.nota;
    

        desempenhoCriacao->transferenciasLeitura += 1;
        if(fread(&heap.alunos[0].aluno, sizeof(Aluno), 1, arquivoBinario) == 0) {
            
            heap.alunos[0] = heap.alunos[heap.numeroAlunos - 1];
            heap.numeroAlunos--;

            heapRefaz(heap.alunos, 0, heap.numeroAlunos, desempenhoCriacao);
        }
        
        else{
            desempenhoCriacao->comparacoes += 1;
            if(heap.alunos[0].aluno.nota < comparaNotaMarcacao)
                heap.alunos[0].marcado = true;
                
            heapRefaz(heap.alunos, 0, heap.numeroAlunos, desempenhoCriacao);
        }
    }

    auto stop = high_resolution_clock::now();
    desempenhoCriacao->tempoExecucao = duration_cast<nanoseconds>(stop - start);

    fclose(arquivoBinario);
}

bool todosMarcados(Heap *heap){

    for(int i = 0; i < heap->numeroAlunos; i++){
        if(heap->alunos[i].marcado == false)
            return false;
    }

    return true;
}

void desmarcaTodos(Heap *heap){

    for(int i = 0; i < heap->numeroAlunos; i++){
        heap->alunos[i].marcado = false;
    }
}