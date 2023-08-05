#include "heap.h"

void heapConstroi(Heap *heap, Desempenho *desempenhoCriacao) {
    
    // Primeiro nó antes do nó folha do heap
    int esquerda = (heap->numeroAlunos / 2) - 1;

    while(esquerda >= 0) {
        heapRefaz(heap->alunos, esquerda, heap->numeroAlunos - 1, desempenhoCriacao);
        esquerda--;
    }
}

void heapRefaz(AlunoHeap *alunos, int esquerda, int direita, Desempenho *desempenhoCriacao) {

    int i = esquerda, j = i * 2 + 1;
    AlunoHeap auxiliar = alunos[i];

    while(j <= direita) {

        if(j < direita && comparaAlunosHeap(&alunos[j], &alunos[j + 1], desempenhoCriacao))
            j = j + 1;

        if(!comparaAlunosHeap(&auxiliar, &alunos[j], desempenhoCriacao))
            break;

        alunos[i] = alunos[j];
        i = j;
        j = i * 2 + 1;
    }

    alunos[i] = auxiliar;
}

// Compara dois alunos e retorna true se a nota do primeiro for maior.
// Funciona tipo o <
// Sobrecarga paraguai do <
bool comparaAlunosHeap(AlunoHeap *aluno1, AlunoHeap *aluno2, Desempenho *desempenhoCriacao) {
    
    desempenhoCriacao->comparacoes += 1;
    
    // Altas chances de falhar
    // Testamos e comprovamos que true > false
    if(aluno1->marcado < aluno2->marcado)
        return false;
        
    else if(aluno1->marcado > aluno2->marcado)
        return true;
    
    else {
        if(aluno1->aluno.nota < aluno2->aluno.nota)
            return false;
    
        else
            return true;

    /*
        if(aluno1->aluno.nota <= aluno2->aluno.nota )
            return true;
    
        else
            return false;
    */
    }
}

void imprimeHeap(Heap *heap) {
    
    cout << "Número de alunos no heap: " << heap->numeroAlunos << endl;

    for(int i = 0; i < heap->numeroAlunos; i++){
        
        if(heap->alunos[i].marcado)
            cout << "*";

        cout << heap->alunos[i].aluno.nota << " | ";
    }

    cout << "\n\n";
}