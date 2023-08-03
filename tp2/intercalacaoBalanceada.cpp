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

    bool fitaIntercalada = false;
    int lixo;

    while(continuaIntercalacao(fitas)){
        // cout << "AAAAAAA" << endl; 
        reiniciaPonteirosFitas(fitas);
        int numeroBlocos = maxBlocos(fitas);
        apagaFitasEntrada(fitas, fitaIntercalada);

        cout << "Numero de blocos: " << numeroBlocos << endl;
        
        for(int i = 0; i < numeroBlocos; i++){
            intercala(fitas, i + 1, fitaIntercalada);
        }

        fitaIntercalada = !fitaIntercalada;

        reiniciaPonteirosFitas(fitas);
        imprimeFitas(fitas);
        cin >> lixo;


    }

    // reiniciaPonteirosFitas(fitas);
    
    //
    // imprimeFitas(fitas);

    // Fecha todos os arquivos de fitas
    fechaFitas(fitas);

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

bool intercala(Fita* fitas, int blocoAIntercalar, bool fitaIntercalada){

    int lixo;

    int vetorControle[NUMERO_FITAS / 2];
    Bloco alunos; // Memória principal

    // Calcula em qual fita a gente deve escrever
    // BlocoAIntercalar - 1 é pq a gente precisa do contador, mas estamos passando o contador + 1 na função de fora
    // *Essa conta está explicada mais detalhadamente no PC do Bruno*
    // FUNCIONA !!! (JÁ FOI TESTADO, SE DEU ERRADO FOI NA HORA DE INVERTER)
    int fitaEscrita = ((((blocoAIntercalar - 1)  % (NUMERO_FITAS / 2)) + (NUMERO_FITAS / 2)) % (NUMERO_FITAS));

    if(fitaIntercalada)
        fitaEscrita -= NUMERO_FITAS / 2;

    
    for(int i = 0; i < NUMERO_FITAS / 2; i++){
        
        if(fitas[i].numeroBlocos < blocoAIntercalar){
            vetorControle[i] = 0;
            continue;   
        }
        fread(&(vetorControle[i]), sizeof(int), 1, fitas[i].arquivo);
         //vetorControle[i]++;
    }

    // cout << "-----------VETOR CONTROLE INICIO\n";
    // for(int i = 0 ; i < NUMERO_FITAS / 2; i++){
        // cout << vetorControle[i] << "  ";
    // }
    // cout << "------------------------\n";
    //Apaga o conteúdo da fita
    fitas[fitaEscrita].arquivo = freopen(NULL,"wb+",fitas[fitaEscrita].arquivo);

    int soma = somaVetorControle(vetorControle);

    fitas[fitaEscrita].numeroBlocos++; // APENAS INCREMENTAR! NÃO DEFINIR COMO 1.
    fwrite(&soma, sizeof(int), 1, fitas[fitaEscrita].arquivo);


    for(int i = 0; i < NUMERO_FITAS / 2; i++){
        if(vetorControle[i] > 0){
            fread(&alunos.alunos[i], sizeof(Aluno), 1, fitas[i].arquivo);
            vetorControle[i]--;
        }
    }

    while(somaVetorControle(vetorControle) > 0){

        cout << "MEMORIA INTERNA \n";
        for(int i = 0; i < NUMERO_FITAS / 2; i++){
            cout << i <<". Valor controle: "<< vetorControle[i] << "\t Nota: " << alunos.alunos[i].nota << endl;
        }
        cout << "---------------\n";



        int indiceMenorElemento = menorElemento(&alunos, vetorControle);
        cout << "Menor Elemento:  " << indiceMenorElemento << endl;
        //cin >> lixo;

        Aluno aluno = alunos.alunos[indiceMenorElemento];

        fwrite(&aluno, sizeof(Aluno), 1, fitas[fitaEscrita].arquivo);
        
        if(vetorControle[indiceMenorElemento] >= 0){
            fread(&alunos.alunos[indiceMenorElemento], sizeof(Aluno), 1, fitas[indiceMenorElemento].arquivo);
            vetorControle[indiceMenorElemento]--;
        }   
    }



    return true;
}

int menorElemento(Bloco *memoriaPrincipal, int *vetorControle){

    int indiceMenor;

    for(int i = 0; i < NUMERO_FITAS; i++){
        if(vetorControle[i] > 0){
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

int maxBlocos(Fita *fitas){

    int maxBlocos = -1;
    
    for(int i = 0; i < NUMERO_FITAS; i++){
        if(fitas[i].numeroBlocos > maxBlocos )
            maxBlocos = fitas[i].numeroBlocos;
    }

    return maxBlocos;
}

// retângulo laranja nos trêses
int totalAlunosBlocoAIntercalar(Fita *fitas){

    int totalAlunos = 0;  
    int alunosBloco = 0;  

    // Percorre cada fita e lê quantos alunos tem em cada um dos blocos que estão sendo intercalados no momento
    // Soma essa quantidade em uma variável que vai ser retornada na função
    for(int i = 0; i < NUMERO_FITAS; i++){

        // Lê a quantidade de alunos no bloco
        fread(&alunosBloco, sizeof(int), 1, fitas[i].arquivo);

        // Volta a posição do ponteiro para antes do número lido, para não atrapalhar a outra função
        fseek(fitas[i].arquivo, (-1 * sizeof(int)), SEEK_CUR);

        // Soma a quantidade de alunos no acumulador
        totalAlunos += alunosBloco;
    }

    return totalAlunos;

}

void reiniciaPonteirosFitas(Fita* fitas){

    for(int i = 0; i < NUMERO_FITAS; i++){
        rewind(fitas[i].arquivo);
    }

}

void apagaFitasEntrada(Fita* fitas, bool fitasAlternada){

    for(int i = 0;i < NUMERO_FITAS / 2; i++){
        if(fitasAlternada){
            fitas[i].arquivo = freopen(NULL,"wb+",fitas[i].arquivo);
            fitas[i].numeroBlocos = 0;
        }
        
        else {
            fitas[i + 20].arquivo = freopen(NULL,"wb+",fitas[i + 20].arquivo);
            fitas[i + 20].numeroBlocos = 0;
        }
    }
}

int somaVetorControle(int *vetorControle){

    int soma = 0;

    for(int i = 0; i < (NUMERO_FITAS / 2); i++){
        soma += vetorControle[i];
    }
    
    return soma;
    
}

bool continuaIntercalacao(Fita *fitas){
    int numeroBlocos = 0;
    for(int i = 0; i < NUMERO_FITAS / 2; i++){
        numeroBlocos += fitas[i].numeroBlocos;

        if(numeroBlocos > 1)
            return true;
    }
    
    return false;
}

void fechaFitas (Fita* fitas){

    for(int i = 0; i < NUMERO_FITAS; i++){
        fclose(fitas[i].arquivo);
    }

}







// Qual fita escrever
// ((contador + 3) % 6) + 1
// ((contador + numeroFitas) % (2*numeroFitas)) + 1

// int fitaEscrita = ((((blocoAIntercalar % (NUMERO_FITAS / 2)) - 1) + (NUMERO_FITAS / 2)) % (NUMERO_FITAS)) + 1;


// 17 é nois