// Esse arquivo é responsável por definir as funções utilizada nos processo de intercalação da ordenação externa

#include <stdio.h>

#include "aluno.h"
#include "intercalacaoBalanceada.h"
#include "merge.h"
#include "binarios.h"
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
    
    // Equanto ainda existirem blocos a serem intercalados, continua o processo de intercalação
    // (cada iteração desse loop realiza a intercalação de todos os blocos das fitas de entrada)
    while(continuaIntercalacao(fitas, fitaIntercalada)) {

        flushFitas(fitas);
        reiniciaPonteirosFitas(fitas);

        // Apaga o conteúdo das fitas de saída, já que elas não serão mais usadas nesse processo, só as de entrada
        apagaFitasSaida(fitas, fitaIntercalada);
        
        // Pega o número de blocos da fita que tem mais blocos
        int numeroBlocos = maxBlocos(fitas, fitaIntercalada);

        // Realiza a intercalação dos blocos, cada iteração desse loop realiza a intercalação completa de blocos
        // que estão no mesmo índice da fita. Ex: na primeira iteração intercala o primeiro bloco de todas as fitas de entrada,
        // na segunda, os segundos, e assim por diante...
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
    // (((contador % f fitas) + f fitas) % (2f fitas))
    // BlocoAIntercalar - 1 é pq a gente precisa do contador, mas estamos passando o contador + 1 na função de fora
    // *Essa conta está explicada mais detalhadamente no PC do Bruno*
    int fitaEscrita = ((((blocoAIntercalar - 1) % (NUMERO_FITAS / 2)) + (NUMERO_FITAS / 2)) % (NUMERO_FITAS));

    // Recalcula qual fita deve ser escrita se as fitas de entrada virarem as fitas de saída e vice-versa.
    if(fitaIntercalada)
        fitaEscrita -= (NUMERO_FITAS / 2);
 
    // Calcula de onde começam as fitas de entrada, caso haja a intercalação, as fitas de saída viram as de entrada
    int inicioFitasEntrada = 0;
    if(fitaIntercalada)
        inicioFitasEntrada = 20;

    // Percorre as fitas de entrada
    for(int i = inicioFitasEntrada; i < inicioFitasEntrada + (NUMERO_FITAS / 2); i++) {
        
        // Verifica se a fita possui o bloco que está sendo ordenado no momento,
        // caso ela não possua, a fita não é usada nessa iteração da intercalação,
        // recebendo 0 no vetor de controle que indica isso.
        if(fitas[i].numeroBlocos < blocoAIntercalar) {
            vetorControle[i] = 0;
            continue;
        }

        // Caso a fita possua o bloco a ser ordenado nessa etapa, lê a quantidade de alunos
        // que a fita tem nesse bloco, e quarda no vetor de controle
        fread(&(vetorControle[i % (NUMERO_FITAS / 2)]), sizeof(int), 1, fitas[i].arquivo);
        desempenhoIntercalacao->transferenciasLeitura += 1;
    }

    // Pega a soma do vetor de controle, que vai ser a quantidade de alunos totais a serem
    // intercalados na presente etapa de intercalação do bloco. Com essa soma, é possível
    // escrever a quantidade de alunos no bloco resultante da intercalação na fita de saída mais facilmente
    int soma = somaVetorControle(vetorControle);

    // Incremente a quantidade de blocos na fita de saída, já que estamos prestes a escrever um bloco
    fitas[fitaEscrita].numeroBlocos++; 

    // Escreve a quantidade de alunos no bloco da fita de saída (soma explicada anteriormente)
    fwrite(&soma, sizeof(int), 1, fitas[fitaEscrita].arquivo);
    desempenhoIntercalacao->transferenciasEscrita += 1;

    // Percorre as fitas de entrada
    for(int i = inicioFitasEntrada; i < inicioFitasEntrada + (NUMERO_FITAS / 2); i++) {

        // Se ainda houverem alunos para ler no bloco da fita de entrada, lê o aluno,
        // seta o aluno como válido e decrementa o número de alunos que precisam ler lido do bloco da fita de entrada
        if(vetorControle[i % (NUMERO_FITAS / 2)] > 0) {

            fread(&alunos[i % (NUMERO_FITAS / 2)], sizeof(Aluno), 1, fitas[i].arquivo);
            desempenhoIntercalacao->transferenciasLeitura += 1;

            vetorAlunoValido[i % (NUMERO_FITAS / 2)] = 1;
        
            vetorControle[i % (NUMERO_FITAS / 2)]--;

        }

    }

    // Equanto houverem alunos válidos no vetor de alunos válidos
    while(temAlunoValido(vetorAlunoValido)) {

        int offsetLeituraAlteranada = 0;

        if(fitaIntercalada)
            offsetLeituraAlteranada = NUMERO_FITAS / 2;

        // Descobre qual é o índice do menor aluno em memória principal
        int indiceMenorElemento = menorElemento(alunos, vetorAlunoValido, desempenhoIntercalacao);

        // Pega o menor aluno
        Aluno aluno = alunos[indiceMenorElemento];

        // Escreve o aluno na fita de saída e invalida a posição no vetor, por enquanto
        fwrite(&aluno, sizeof(Aluno), 1, fitas[fitaEscrita].arquivo);
        desempenhoIntercalacao->transferenciasEscrita += 1;
        vetorAlunoValido[indiceMenorElemento] = 0;
        
        // Se ainda houverem mais alunos para serem lidos na fita de entrada do aluno que acabamos de escrever,
        // lemos mais um aluno, setamos a posição do vetor como válida novamente, e decrementamos a quantidade de alunos
        // que precisam ser lidos desse bloco dessa fita de entrada
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

    // começa o menor aluno como sendo o primeiro aluno válido do vetor
    for(int i = 0; i < NUMERO_FITAS; i++) {
        if(vetorAlunoValido[i] > 0) {
            indiceMenor = i;
            break;
        }
    }

    // Percorre a memória interna, a partir do menor aluno definido anteriormente,
    // e descobre qual é o menor aluno, retornando o índice dele no final
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
