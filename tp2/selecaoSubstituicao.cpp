#include <stdio.h>
#include <limits>

#include "heap.h"
#include "selecaoSubstituicao.h"

using namespace std;

void criaBlocosSelecaoSubstituicao(Fita *fitas, char *nomeArquivoBinario, Desempenho *desempenhoCriacao){

    int lixo;

    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }

    Heap heap;
    int numeroFitaAtual = 0;
    int numeroAlunosFitaAtual = 0;

    // Pega o número de alunos da fita atual
    fwrite(&numeroAlunosFitaAtual, sizeof(int), 1, fitas[numeroFitaAtual].arquivo);
    fitas[numeroFitaAtual].numeroBlocos++;
    
    desempenhoCriacao->transferenciasEscrita += 1;

    // Marcado quando é menor.
    //bool vetorTags[NUMERO_FITAS / 2] = {false};
    
    // Escreve o total de alunos no heap
    int alunoAtual = 0;
    
    while(fread(&heap.alunos[alunoAtual++].aluno, sizeof(Aluno), 1, arquivoBinario) && alunoAtual < NUMERO_FITAS / 2) {
        desempenhoCriacao->transferenciasLeitura += 1;
        heap.numeroAlunos = alunoAtual;
        heap.alunos[alunoAtual - 1].marcado = false;
    }
 
    heapConstroi(&heap, desempenhoCriacao);

    while(heap.numeroAlunos > 0){ // enquanto tiver alunos no heap

        // cin >> lixo;
        imprimeHeap(&heap);

        // Se todos os alunos forem menores, desmarca todos e constrói o heap
        if(todosMarcados(&heap)){
            desmarcaTodos(&heap);
            heapConstroi(&heap, desempenhoCriacao);
            numeroFitaAtual++;
            fitas[numeroFitaAtual].numeroBlocos += 1;
            numeroAlunosFitaAtual = 0;
            fwrite(&numeroAlunosFitaAtual, sizeof(int), 1, fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo);
        }

        //cout << "Escrevi o aluno-------------------------- " << heap.alunos[0].aluno.nota << endl;  

        cout << "Posicao do ponteiro antes de escrever o aluno = " << ftell(fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo) << endl;
        // Escreve o menor aluno (sempre a posição 0 do heap) na fita e incrementa o número de alunos no bloco
        fwrite(&heap.alunos[0].aluno, sizeof(Aluno), 1, fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo);
        numeroAlunosFitaAtual++;

        desempenhoCriacao->transferenciasEscrita += 1;
        
        //cout << "numeroAlunosFitaAtual = " << numeroAlunosFitaAtual << endl;
        int posicaoInteiroFita = ftell(fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo) - (numeroAlunosFitaAtual * sizeof(Aluno)) - sizeof(int);
        //cout << "PosicaoInteiroFita = " << posicaoInteiroFita << endl;

        // Volta para o número que tem no começo do bloco falando a quantidade de alunos
        fseek(fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo, -(numeroAlunosFitaAtual * sizeof(Aluno)) - sizeof(int), SEEK_END);
        //cout << "Posicao Ponteiro na Fita = " << ftell(fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo);
        // e escreve a quantidade atualizada depois de escrever o aluno

        //cout << "Escrevi o inteiro_______________" << numeroAlunosFitaAtual << endl;

        // Escrevendo o número de alunos na fita
        fwrite(&numeroAlunosFitaAtual, sizeof(int), 1,  fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo);
        
        desempenhoCriacao->transferenciasEscrita += 1;
        fseek(fitas[numeroFitaAtual % (NUMERO_FITAS / 2)].arquivo, 0 , SEEK_END);  

        double comparaNotaMarcacao = heap.alunos[0].aluno.nota;
    
        desempenhoCriacao->transferenciasLeitura += 1;
        if(fread(&heap.alunos[0].aluno, sizeof(Aluno), 1, arquivoBinario) == 0) {
            heap.alunos[0].aluno.nota = numeric_limits<double>::infinity();
            heapRefaz(heap.alunos, 0, heap.numeroAlunos, desempenhoCriacao);
            heap.numeroAlunos--;    
        } 
        
        else{
            if(heap.alunos[0].aluno.nota < comparaNotaMarcacao)
                heap.alunos[0].marcado = true;
                
            heapRefaz(heap.alunos, 0, heap.numeroAlunos, desempenhoCriacao);
        }

    cout << "Fita atual : " << numeroFitaAtual << "   Número de alunos na fita: " << numeroAlunosFitaAtual << endl;
    }

    cout << "Tamanho da fita 0: " << ftell(fitas[0].arquivo) << endl;
    cout << "Tamanho da fita 1: " << ftell(fitas[1].arquivo) << endl;
    cout << "Tamanho da fita 2: " << ftell(fitas[2].arquivo) << endl;

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