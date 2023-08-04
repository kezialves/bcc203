#include <stdio.h>

#include "aluno.h"
#include "intercalacaoBalanceada.h"
#include "merge.h"
#include "converteBin.h"

bool ordenaIntercalacaoBalanceada(Argumentos *argumentos, char *nomeArquivoBinario, Desempenho *desempenho){

    int lixo;

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

    bool fitaIntercalada = false;
    
    while(continuaIntercalacao(fitas, fitaIntercalada)) {

        flushFitas(fitas);
        reiniciaPonteirosFitas(fitas);

        apagaFitasSaida(fitas, fitaIntercalada);
        
        int numeroBlocos = maxBlocos(fitas);

        for(int i = 0; i < numeroBlocos; i++) {
            intercala(fitas, i + 1, fitaIntercalada);
            flushFitas(fitas);
        }

        fitaIntercalada = !fitaIntercalada;

        reiniciaPonteirosFitas(fitas);
        imprimeFitas(fitas);

        flushFitas(fitas);

        cin >> lixo;
    }

    int fitaInicial = 0;

    if(!fitaIntercalada)
        fitaInicial += 20;
    
    reiniciaPonteirosFitas(fitas);

    // Acha a fita final ordenada e converte para txt

    for(int i = fitaInicial; i < NUMERO_FITAS; i++) {

        if(fitas[i].numeroBlocos > 0) {
            cout << "Fita de saída: " << i << endl;
            
            Aluno aluno;
            FILE *arqT;
            arqT = fopen("fitaOrdenada.txt", "w");
            
            rewind(fitas[i].arquivo);
            fread(&lixo, sizeof(int), 1, fitas[i].arquivo);
            
            while(fread(&aluno, sizeof(Aluno), 1 ,fitas[i].arquivo)) {
                fprintf(arqT, "%08ld %04.1f %s %-25s %40s\n", aluno.numeroInscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso);
            }
            
            break;
        }
    }

    // Fecha todos os arquivos de fitas
    fechaFitas(fitas);

    return true;
}

bool intercala(Fita* fitas, int blocoAIntercalar, bool fitaIntercalada){

    int lixo;

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

    cout << "Intercala " << blocoAIntercalar << " -----> Fita escrita: " << fitaEscrita << endl;

    // cout << "Tamanho int: " << sizeof(int) << endl;
    // cout << "Tamanho Aluno: " << sizeof(Aluno) << endl;
 
    int inicioFitasEntrada = 0;

    if(fitaIntercalada)
        inicioFitasEntrada = 20;

    for(int i = inicioFitasEntrada; i < inicioFitasEntrada + (NUMERO_FITAS / 2); i++) {
        
        if(fitas[i].numeroBlocos < blocoAIntercalar) {
            vetorControle[i] = 0;
            continue;
        }

        // cout << "Posição ponteiro arquivo " << i << ": " << ftell(fitas[i].arquivo) << endl;

        fread(&(vetorControle[i % (NUMERO_FITAS / 2)]), sizeof(int), 1, fitas[i].arquivo);

        // cout << "Número alunos na fita " << i << ": " << vetorControle[i] << endl;
        // vetorControle[i]++;
    }

    // Printa o vetor de controle
    cout << "Vetor de controle: [";
    for(int i = 0; i < (NUMERO_FITAS / 2); i++) {
        cout << vetorControle[i] << " ";
    }
    cout << "]" << endl;
    
    // Apaga o conteúdo da fita
    fitas[fitaEscrita].arquivo = freopen(NULL,"wb+",fitas[fitaEscrita].arquivo);

    int soma = somaVetorControle(vetorControle);

    fitas[fitaEscrita].numeroBlocos++; // APENAS INCREMENTAR! NÃO DEFINIR COMO 1.

    cout << "Soma dos alunos: " << soma << " na fita " << fitaEscrita << endl; 

    fwrite(&soma, sizeof(int), 1, fitas[fitaEscrita].arquivo);
    
    if(fitaIntercalada == true && blocoAIntercalar == 1) {
        fseek(fitas[0].arquivo, 0, SEEK_END);
        cout << "TAMANHO FITAS" << ftell(fitas[0].arquivo) << endl;
        cout << "FRWITE, ERA PRA ESCREVER NA 0 COM O FWRUTE_____________" << endl;
    }

    // ++ no vetor de controle
    // for(int i = 0; i < (NUMERO_FITAS / 2); i++) {
    //     vetorControle[i]++;
    // }

    for(int i = inicioFitasEntrada; i < inicioFitasEntrada + (NUMERO_FITAS / 2); i++) {
        if(vetorControle[i % (NUMERO_FITAS / 2)] > 0) {
            fread(&alunos[i % (NUMERO_FITAS / 2)], sizeof(Aluno), 1, fitas[i].arquivo);
            vetorAlunoValido[i % (NUMERO_FITAS / 2)] = 1;
            vetorControle[i % (NUMERO_FITAS / 2)]--;
        }
    }

    int contadorarbitrario = 0;
    
    while(temAlunoValido(vetorAlunoValido)) {

        int offsetLeituraAlteranada = 0;

        if(fitaIntercalada)
            offsetLeituraAlteranada = 20;


        contadorarbitrario++;

        int indiceMenorElemento = menorElemento(alunos, vetorAlunoValido);

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
        vetorAlunoValido[indiceMenorElemento] = 0; 
        
        if(vetorControle[indiceMenorElemento] > 0) {
            fread(&alunos[indiceMenorElemento], sizeof(Aluno), 1, fitas[indiceMenorElemento + offsetLeituraAlteranada].arquivo);
            vetorAlunoValido[indiceMenorElemento] = 1;
            vetorControle[indiceMenorElemento]--;
        }

        // if(fitaIntercalada)
        //     cin >> lixo;
    }

    if(fitaIntercalada == true && blocoAIntercalar == 1) {
        fseek(fitas[0].arquivo, 0, SEEK_END);
        cout << "TAMANHO FITA 21: " << ftell(fitas[21].arquivo) << endl;
        cout << "FRWITE, ERA PRA ESCREVER NA 0 COM O FWRUTE_____________" << endl;
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

int menorElemento(Aluno *alunos, int *vetorAlunoValido) {

    int indiceMenor;

    for(int i = 0; i < NUMERO_FITAS; i++) {
        if(vetorAlunoValido[i] > 0) {
            indiceMenor = i;
            break;
        }
    }

    for(int i = indiceMenor + 1; i < TAMANHO_MEMORIA_INTERNA; i++) {
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

// Blocos --------------------------------------------------

void criaBlocosOrdenacaoInterna(Fita *fitas, Desempenho *desempenho, char *nomeArquivoBinario) {

    FILE *arquivoBinario;

    // Retorna caso não seja possível abrir o arquivo binário
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }
    
    Bloco blocoLido;
    int numeroFitaAtual = 0;

    // Lê os alunos enquanto a quantidade de alunos é válida
    do {
        blocoLido.numeroAlunos = fread(blocoLido.alunos, sizeof(Aluno), TAMANHO_INICIAL_BLOCO, arquivoBinario);
        // cout << "Número de alunos: " << blocoLido.numeroAlunos << endl;

        // Ordena o bloco lido internamente
        mergeSort(blocoLido.alunos, 0, blocoLido.numeroAlunos - 1);

        // cout << "--------------------\n";

        // for(int i = 0; i < blocoLido.numeroAlunos; i++) {
        //     cout << blocoLido.alunos[i].nota << endl;
        // }

        // cout << "--------------------\n";

        // cout << "Número da fita atual: " << numeroFitaAtual << endl;

        // Se o bloco é válido, adiciona-o na fita
        if(blocoLido.numeroAlunos > 0) {
            adicionaBloco(&fitas[numeroFitaAtual % 20], &blocoLido);
            numeroFitaAtual++;
        }
    } while(blocoLido.numeroAlunos != 0);
}

int maxBlocos(Fita *fitas) {

    int maxBlocos = -1;
    
    for(int i = 0; i < NUMERO_FITAS; i++) {
        if(fitas[i].numeroBlocos > maxBlocos)
            maxBlocos = fitas[i].numeroBlocos;
    }

    return maxBlocos;
}

// Fitas --------------------------------------------------

void reiniciaPonteirosFitas(Fita *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {
        rewind(fitas[i].arquivo);
    }
}

void apagaFitasSaida(Fita *fitas, bool fitasAlternada) {

    for(int i = 0; i < NUMERO_FITAS / 2; i++) {

        // Se as fitas foram alternadas, as de saída são as f primeiras
        if(fitasAlternada) {
            fitas[i].arquivo = freopen(NULL, "wb+", fitas[i].arquivo);
            fitas[i].numeroBlocos = 0;
        }
        
        // Senão, são as f últimas
        else {
            fitas[i + 20].arquivo = freopen(NULL, "wb+", fitas[i + 20].arquivo);
            fitas[i + 20].numeroBlocos = 0;
        }
    }
}

void fechaFitas(Fita *fitas) {

    for(int i = 0; i < NUMERO_FITAS; i++) {
        fclose(fitas[i].arquivo);
    }
}

// Qual fita escrever
// ((contador + 3) % 6) + 1
// ((contador + numeroFitas) % (2*numeroFitas)) + 1

// int fitaEscrita = ((((blocoAIntercalar % (NUMERO_FITAS / 2)) - 1) + (NUMERO_FITAS / 2)) % (NUMERO_FITAS)) + 1;