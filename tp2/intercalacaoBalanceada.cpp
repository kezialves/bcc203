#include <stdio.h>

#include "aluno.h"
#include "intercalacaoBalanceada.h"
#include "merge.h"
#include "converteBin.h"
#include "ordenacaoInterna.h"
#include "selecaoSubstituicao.h"

bool ordenaIntercalacaoBalanceada(Argumentos *argumentos, char *nomeArquivoBinario, Desempenho *desempenhoCriacao, Desempenho *desempenhoIntercalacao) {

    Fita fitas[NUMERO_FITAS];
    iniciaFitas(fitas);
    
    // Cria os blocos inicialmente ordenados
    switch(argumentos->metodoOrdenacao){

        // Cria os blocos por ordenação interna através do mergeSort
        case 1:
            criaBlocosOrdenacaoInterna(fitas, nomeArquivoBinario, desempenhoCriacao);
            break;
        
        // Cria os blocos através da seleção por substituição
        case 2:
            criaBlocosSelecaoSubstituicao(fitas, nomeArquivoBinario, desempenhoCriacao);
            // reiniciaPonteirosFitas(fitas);
            break;
    }

    bool fitaIntercalada = false;
    
    auto start = high_resolution_clock::now();
    
    while(continuaIntercalacao(fitas, fitaIntercalada)) {

        flushFitas(fitas);
        reiniciaPonteirosFitas(fitas);

        apagaFitasSaida(fitas, fitaIntercalada);
        
        int numeroBlocos = maxBlocos(fitas, fitaIntercalada);

        for(int i = 0; i < numeroBlocos; i++) {
            intercala(fitas, i + 1, fitaIntercalada, desempenhoIntercalacao);
            flushFitas(fitas);
        }

        fitaIntercalada = !fitaIntercalada;
        reiniciaPonteirosFitas(fitas);
        
        // Caso o -P seja passado em tempo de compilação, imprime o processo
        if(argumentos->p)
            imprimeFitas(fitas);

        flushFitas(fitas);
    }

    auto stop = high_resolution_clock::now();
    desempenhoIntercalacao->tempoExecucao = duration_cast<nanoseconds>(stop - start);

    int fitaInicial = 0;

    if(fitaIntercalada)
        fitaInicial += NUMERO_FITAS / 2;
    
    reiniciaPonteirosFitas(fitas);

    // Acha a fita final ordenada e converte para txt

    for(int i = fitaInicial; i < fitaInicial + NUMERO_FITAS / 2; i++) {

        if(fitas[i].numeroBlocos > 0) {
            Aluno aluno;
            FILE *arqT;
            arqT = fopen("fitaOrdenada.txt", "w");
            rewind(fitas[i].arquivo);

            int numeroAlunos;
            fread(&numeroAlunos, sizeof(int), 1, fitas[i].arquivo);
            
            while(fread(&aluno, sizeof(Aluno), 1 ,fitas[i].arquivo)) {
                fprintf(arqT, "%08ld %04.1f %s %-25s %40s\n", aluno.numeroInscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
            }
            
            fclose(arqT);
            break;
        }
    }

    // Fecha todos os arquivos de fitas
    fechaFitas(fitas);

    return true;
}

bool intercala(Fita* fitas, int blocoAIntercalar, bool fitaIntercalada, Desempenho *desempenhoIntercalacao) {

    int vetorControle[NUMERO_FITAS / 2];
    int vetorAlunoValido[NUMERO_FITAS / 2];

    for(int i = 0; i < (NUMERO_FITAS / 2); i++) {
       vetorAlunoValido[i] = 0;
    }

    Aluno alunos[20]; // Memória principal

    // Calcula em qual fita a gente deve escrever
    // ((contador + f fitas) % (2f fitas)) + 1
    // BlocoAIntercalar - 1 é pq a gente precisa do contador, mas estamos passando o contador + 1 na função de fora
    // *Essa conta está explicada mais detalhadamente no PC do Bruno*
    // FUNCIONA!!! (JÁ FOI TESTADO, SE DEU ERRADO FOI NA HORA DE INVERTER)
    int fitaEscrita = ((((blocoAIntercalar - 1) % (NUMERO_FITAS / 2)) + (NUMERO_FITAS / 2)) % (NUMERO_FITAS));

    if(fitaIntercalada)
        fitaEscrita -= (NUMERO_FITAS / 2);
 
    int inicioFitasEntrada = 0;

    if(fitaIntercalada)
        inicioFitasEntrada = 20;

    for(int i = inicioFitasEntrada; i < inicioFitasEntrada + (NUMERO_FITAS / 2); i++) {
        
        if(fitas[i].numeroBlocos < blocoAIntercalar) {
            vetorControle[i] = 0;
            continue;
        }

        fread(&(vetorControle[i % (NUMERO_FITAS / 2)]), sizeof(int), 1, fitas[i].arquivo);
        desempenhoIntercalacao->transferenciasLeitura += 1;
    }

    int soma = somaVetorControle(vetorControle);

    fitas[fitaEscrita].numeroBlocos++; // APENAS INCREMENTAR! NÃO DEFINIR COMO 1.

    fwrite(&soma, sizeof(int), 1, fitas[fitaEscrita].arquivo);
    desempenhoIntercalacao->transferenciasEscrita += 1;

    for(int i = inicioFitasEntrada; i < inicioFitasEntrada + (NUMERO_FITAS / 2); i++) {
        if(vetorControle[i % (NUMERO_FITAS / 2)] > 0) {
            fread(&alunos[i % (NUMERO_FITAS / 2)], sizeof(Aluno), 1, fitas[i].arquivo);
            desempenhoIntercalacao->transferenciasLeitura += 1;
            vetorAlunoValido[i % (NUMERO_FITAS / 2)] = 1;
            vetorControle[i % (NUMERO_FITAS / 2)]--;
        }
    }

    int contadorarbitrario = 0;
    
    while(temAlunoValido(vetorAlunoValido)) {

        int offsetLeituraAlteranada = 0;

        if(fitaIntercalada)
            offsetLeituraAlteranada = NUMERO_FITAS / 2;

        contadorarbitrario++;

        int indiceMenorElemento = menorElemento(alunos, vetorAlunoValido, desempenhoIntercalacao);

        // cout << "Menor Elemento: " << indiceMenorElemento << endl;

        // cout << "Vetor controle: [";
        // for(int i = 0; i < 5; i++) {
        //     cout << vetorControle[i] << " ";
        // }
        // cout << "]" << endl;

        // cout << "Vetor alunosValidos: [";
        // for(int i = 0; i < 5; i++) {
        //     cout << vetorAlunoValido[i] << " ";
        // }
        // cout << "]" << endl;

        // cout << "Vetor de alunos: [";
        // for(int i = 0; i < 5; i++) {
        //     cout << alunos[i].nota << " ";
        // }
        // cout << "]" << endl;
       
        Aluno aluno = alunos[indiceMenorElemento];

        fwrite(&aluno, sizeof(Aluno), 1, fitas[fitaEscrita].arquivo);
        desempenhoIntercalacao->transferenciasEscrita += 1;
        vetorAlunoValido[indiceMenorElemento] = 0;
        
        if(vetorControle[indiceMenorElemento] > 0) {
            fread(&alunos[indiceMenorElemento], sizeof(Aluno), 1, fitas[indiceMenorElemento + offsetLeituraAlteranada].arquivo);
            desempenhoIntercalacao->transferenciasLeitura += 1;
            vetorAlunoValido[indiceMenorElemento] = 1;
            vetorControle[indiceMenorElemento]--;
        }
    }

    flushFitas(fitas);
    return true;
}

bool continuaIntercalacao(Fita *fitas, bool fitaIntercalada) {

    int numeroBlocos = 0;
    int inicioFitasSaida;
    
    if(fitaIntercalada)
        inicioFitasSaida = NUMERO_FITAS / 2;
    
    else
        inicioFitasSaida = 0;

    for(int i = inicioFitasSaida; i < inicioFitasSaida + (NUMERO_FITAS / 2); i++) {
        numeroBlocos += fitas[i].numeroBlocos;

        // Quando há mais de um bloco, retorna true para intercalá-los
        if(numeroBlocos > 1)
            return true;
    }
    
    // Se só há um bloco, não tem mais o quê intercalar
    return false;
}

int menorElemento(Aluno *alunos, int *vetorAlunoValido, Desempenho *desempenhoIntercalacao) {

    int indiceMenor;

    for(int i = 0; i < NUMERO_FITAS; i++) {
        if(vetorAlunoValido[i] > 0) {
            indiceMenor = i;
            break;
        }
    }

    for(int i = indiceMenor + 1; i < TAMANHO_MEMORIA_INTERNA; i++) {

        desempenhoIntercalacao->comparacoes += 1;

        // mudar de < para > se quiser ordenar descendentemente, mas tem que mudar o merge também 
        if(alunos[i].nota < alunos[indiceMenor].nota && vetorAlunoValido[i] > 0) {
            indiceMenor = i;
        }
    }

    return indiceMenor;
}

int somaVetorControle(int *vetorControle) {

    int soma = 0;

    for(int i = 0; i < (NUMERO_FITAS / 2); i++) {
        soma += vetorControle[i];
    }
    
    return soma;
}

bool temAlunoValido(int *vetorAlunoValido) {

    for(int i = 0; i < (NUMERO_FITAS / 2); i++) {
        if(vetorAlunoValido[i] == 1)
            return true;
    }
    
    return false;
}
