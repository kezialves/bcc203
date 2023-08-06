// Esse arquivo é responsável por definir as funções utilizadas no processo de ciração dos blocos inicialmente
// ordenados

#include <stdio.h>

#include "ordenacaoInterna.h"

// Quantidade de escritas na criação dos blocos:
// com 1000 alunos são 10 fitas com 3 blocos e 10 fitas com 2 blocos
// A usa duas escritas para adicionar um bloco. 
// Uma para escrever a quantidade de alunos no bloco e outra para escrever o bloco
// Ou seja, 2 * (3*10) + 2 * (2*20) = 60 + 40 = 100. Com 1000 alunos

// Quantidade de leituras na criação dos blocos:
// com 1000 alunos são 10 fitas com 3 blocos e 10 fitas com 2 blocos
// Nós fazemos uma só leitura para ver quantos alunos tem no bloco
// Ou seja, 50 blocos = 50 leituras, o do-while executa pelo menos uma vez, então +1 = 51

void criaBlocosOrdenacaoInterna(Fita *fitas, char *nomeArquivoBinario, Desempenho *desempenhoCriacao) {

    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }
    
    auto start = high_resolution_clock::now();

    Bloco blocoLido;
    int numeroFitaAtual = 0;

    // Lê um bloco de alunos, ordena e os adicionam na fita.
    // O processo se repete enquanto houverem alunos no arquivo original (enquanto conseguir ler alunos)
    do {

        // Lê o bloco do arquivo original e armazena a quantidade de alunos lidos em uma variável
        blocoLido.numeroAlunos = fread(blocoLido.alunos, sizeof(Aluno), TAMANHO_INICIAL_BLOCO, arquivoBinario);
        desempenhoCriacao->transferenciasLeitura += 1;

        // Ordena o bloco lido internamente
        mergeSort(blocoLido.alunos, 0, blocoLido.numeroAlunos - 1, desempenhoCriacao);

        // Se o bloco é válido (tem alunos no bloco), adiciona-o na fita e passa para a próxima
        if(blocoLido.numeroAlunos > 0) {
            adicionaBloco(&fitas[numeroFitaAtual % 20], &blocoLido, desempenhoCriacao);
            numeroFitaAtual++;
        }

    } while(blocoLido.numeroAlunos != 0);

    auto stop = high_resolution_clock::now();
    desempenhoCriacao->tempoExecucao = duration_cast<nanoseconds>(stop - start);
}