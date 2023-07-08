#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "merge.h"

using namespace std;

void mergeSort(int *vetor, int l, int r) {

    if(l < r) { // o começo tem que ser menor que o final pra dividir
        int m = (r + l) / 2; // calcula o meio do vetor
        mergeSort(vetor, l, m); // passa a primeira metade do vetor
        mergeSort(vetor, m + 1, r); // passa a segunda metade do vetor
        merge(vetor, l, m, r); // conquista o bichinho
    }
}

void merge(int *vetor, int l, int m, int r) {

    int tamanhoL, tamanhoR;

    tamanhoL = (m - l + 1); // tamanho do vetor da esquerda
    tamanhoR = (r - m); // tamanho do vetor da direita

    int *vetorL = aloca(tamanhoL); // aloca o vetor da esquerda
    int *vetorR = aloca(tamanhoR); // aloca o vetor da direita

    // preenche o vetor da esquerda com a primeira metade do original
    for(int i = 0; i < tamanhoL; i++) {
        vetorL[i] = vetor[i + l];
    }

    // preenche o vetor da direita com a segunda metade do original
    for(int j = 0; j < tamanhoR; j++) {
        vetorR[j] = vetor[j + m + 1];
    }

    // reinicia os contadores
    int i = 0; // tem que ficar igual ao tamanho de l
    int j = 0; // tem que ficar igual ao tamanho de r

    // inicializa o k com o valor de l, que é o início do vetor da esquerda
    // para quando k chegar ao tamanho de r, que é o final do vetor da direita
    for(int k = l; k <= r; k++) {
        
        if(i == tamanhoL) // se i tiver chegado ao tamanho de l, o vetor da esquerda acabou
            vetor[k] = vetorR[j++]; // então só resta inserir o vetor da direita, que já está ordenado
        
        else if(j == tamanhoR) // se j tiver chegado ao tamanho de r, o vetor da direita acabou
            vetor[k] = vetorL[i++]; // então só resta inserir o vetor da esquerda, que já está ordenado

        else if(vetorL[i] <= vetorR[j]) // se o valor de l em i for menor que o valor de r em j
            vetor[k] = vetorL[i++]; // o vetor em k recebe l em i e incrementa o i

        else // se o valor de r em j for menor que o valor de l em i
            vetor[k] = vetorR[j++]; // o vetor em k recebe r em j e incrementa o j
    }

    desaloca(vetorL); // desaloca o vetor da esquerda
    desaloca(vetorR); // desaloca o vetor da direita
}

int * aloca(int tamanho) {

    int *vetor = (int*) malloc(tamanho * sizeof(int));

    return vetor;
}

void desaloca(int *vetor) {
    free(vetor);
}