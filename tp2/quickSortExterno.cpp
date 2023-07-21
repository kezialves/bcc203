#include <iostream>
#include <limits>

#include "quickSortExterno.h"
#include "converteBin.h"

using namespace std;

void ordenaQuickSort(char *nomeArquivoBinario, Argumentos *argumentos){

    cout << "NOME: " << nomeArquivoBinario; 

    FILE *arquivoLeituraInferior = NULL,
        *arquivoEscritaInferior = NULL,
        *arquivoLeituraEscritaSuperior = NULL;

    if((arquivoLeituraInferior = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }

    if((arquivoEscritaInferior = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }

    if((arquivoLeituraEscritaSuperior = fopen(nomeArquivoBinario, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }

    quickSortExterno(&arquivoLeituraInferior, &arquivoEscritaInferior, &arquivoLeituraEscritaSuperior, 1, argumentos->quantidadeAlunos);

    fclose(arquivoLeituraInferior);
    fclose(arquivoEscritaInferior);
    fclose(arquivoLeituraEscritaSuperior);

    string nomeSaida = "saidaQuickSortExterno2.txt";
 
    binarioParaTexto(nomeArquivoBinario,nomeSaida.data() , argumentos);

    return;
}

void quickSortExterno(FILE **arquivoLeituraInferior, FILE **arquivoEscritaInferior, 
    FILE **arquivoLeituraEscritaSuperior, int esquerda, int direita) {

    // Utilizamos leitura e escrita superior na mesma variável para economizar no fseek

    // Antigos i, j
    int finalParticaoEsquerda, inicioParticaoDireita;

    // Caso base, significa que não tem elementos para ordenar.
    if(direita - esquerda < 1) {
        // TO-DO!!!
        return;
    }
    
    // Área de armazenamento interno
    Area area;

    // Zera a área de armazenamento interno, garantindo que a área esteja vazia para o começo da partição.
    fazAreaVazia(&area);
    
    // Instancia a partição
    particao(arquivoLeituraInferior, arquivoEscritaInferior, arquivoLeituraEscritaSuperior,
        area, esquerda, direita, &finalParticaoEsquerda, &inicioParticaoDireita);

    // O C tem diversos problemas quando se trata de trabalhar com arquivos
    // Apesar do código ser implementado em C++, as funções que utilizamos com os arquivos são do C
    // Usar o fflush a cada instância ajuda o código a funcionar como deveria
    fflush(*arquivoLeituraInferior);
    fflush(*arquivoEscritaInferior);
    fflush(*arquivoLeituraEscritaSuperior);

    if(finalParticaoEsquerda - esquerda < direita - inicioParticaoDireita) {
        quickSortExterno(arquivoLeituraInferior, arquivoEscritaInferior, arquivoLeituraEscritaSuperior, esquerda, finalParticaoEsquerda);
        quickSortExterno(arquivoLeituraInferior, arquivoEscritaInferior, arquivoLeituraEscritaSuperior, inicioParticaoDireita, direita);
    }
    
    else {
        quickSortExterno(arquivoLeituraInferior, arquivoEscritaInferior,
            arquivoLeituraEscritaSuperior, inicioParticaoDireita, direita);
        quickSortExterno(arquivoLeituraInferior, arquivoEscritaInferior,
            arquivoLeituraEscritaSuperior, esquerda, finalParticaoEsquerda);
    }
}

void particao(FILE **arquivoLeituraInferior, FILE **arquivoEscritaInferior, FILE **arquivoLeituraEscritaSuperior,
    Area area, int esquerda, int direita, int *finalParticaoEsquerda, int *inicioParticaoDireita){

    int leituraSuperior = direita,
        escritaSuperior = direita,
        leituraInferior = esquerda,
        escritaInferior = esquerda,
        numeroAlunosArea = 0;
    
    double limiteInferior = numeric_limits<double>::min(),
           limiteSuperior = numeric_limits<double>::max();

    bool leDireita;
    
    Aluno ultimoAlunoLido, // lido leitura alternada
          alunoMovido; // saiu do pivô e voltou para a memória

    // Posiciona o ponteiro dos arquivos na posição inicial para começar a leitura alternada
    // e escrita de valores menores do que o pivô na parte inferior
    fseek(*arquivoLeituraInferior, (leituraInferior - 1) * sizeof(Aluno), SEEK_SET);
    fseek(*arquivoEscritaInferior, (escritaInferior - 1) * sizeof(Aluno), SEEK_SET);

    // Inicializa os ponteiros com posições que não existem no arquivo
    *finalParticaoEsquerda = esquerda - 1;
    *inicioParticaoDireita = direita + 1;

    // Enquanto os ponteiros de leitura inferior e superior não se cruzam,
    // continua o processo de partição
    while(leituraSuperior >= leituraInferior) {

        if(numeroAlunosArea < TAMANHO_AREA - 1) {
            
            if(leDireita)
                leSuperior(arquivoLeituraEscritaSuperior, &ultimoAlunoLido, &leituraSuperior, &leDireita);
            
            else 
                leInferior(arquivoLeituraInferior, &ultimoAlunoLido, &leituraInferior, &leDireita);
            
            inserirArea(&area, &ultimoAlunoLido, &numeroAlunosArea);
            
            continue;
        }
        
        // Quando os apontadores de leitura e escrita se igualam,
        // quebra a alternância e lê obrigatoriamente o que está igual
        // para não sobrescrever e perder algum dado que não foi lido
        
        if(leituraSuperior == escritaSuperior)
            leSuperior(arquivoLeituraEscritaSuperior, &ultimoAlunoLido, &leituraSuperior, &leDireita);
        
        else if(leituraInferior == escritaInferior)
            leInferior(arquivoLeituraInferior, &ultimoAlunoLido, &leituraInferior, &leDireita);

        // Quando a variável de controle leDireita está ativada, lê a partição da direita
        else if(leDireita)
            leSuperior(arquivoLeituraEscritaSuperior, &ultimoAlunoLido, &leituraSuperior, &leDireita);
        
        // Caso contrário, lê a partição da esquerda
        else
            leInferior(arquivoLeituraInferior, &ultimoAlunoLido, &leituraInferior, &leDireita);

        // Verifica se a nota está entre os limites inferior e superior, não inclusos

        if(ultimoAlunoLido.nota <= limiteInferior) {
            *finalParticaoEsquerda = escritaInferior;
            escreveMinimo(arquivoEscritaInferior, ultimoAlunoLido, &escritaInferior);
            continue;
        }

        if(ultimoAlunoLido.nota >= limiteSuperior) {
            *inicioParticaoDireita = escritaSuperior;
            escreveMaximo(arquivoLeituraEscritaSuperior, ultimoAlunoLido, &escritaSuperior);
            continue;
        }

        // Caso a nota esteja nesse intervalo, o aluno é apto a entrar no pivô
        inserirArea(&area, &ultimoAlunoLido, &numeroAlunosArea);

        if(escritaInferior - esquerda < direita - escritaSuperior) {
            retiraMinimo(&area, &alunoMovido, &numeroAlunosArea);
            escreveMinimo(arquivoEscritaInferior, alunoMovido, &escritaInferior);
            
            limiteInferior = alunoMovido.nota;
        }
        
        else {
            retiraMaximo(&area, &alunoMovido, &numeroAlunosArea);
            escreveMaximo(arquivoLeituraEscritaSuperior, alunoMovido, &escritaSuperior);
            
            limiteSuperior = alunoMovido.nota;
        }
    }

    // Quando os ponteiros de leitura inferior e superior se cruzam,
    // joga o pivô de volta no arquivo
    // e começa a inserir onde o ponteiro de escrita inferior está apontando
    while(escritaInferior <= escritaSuperior){
        retiraMinimo(&area, &alunoMovido, &numeroAlunosArea);
        escreveMinimo(arquivoEscritaInferior, alunoMovido, &escritaInferior);
    }
}

void fazAreaVazia(Area *area) {
    area->numeroAlunos = 0;
}

int obterNumCelOcupadas(Area *area) {
    return area->numeroAlunos;
}

void leInferior(FILE **arquivoLeituraInferior, Aluno *ultimoAlunoLido, int *leituraInferior, bool *leDireita) {
    
    fread(ultimoAlunoLido, sizeof(Aluno), 1, *arquivoLeituraInferior);
    (*leituraInferior)++;
    *leDireita = true;
}

void leSuperior(FILE **arquivoLeituraEscritaSuperior, Aluno *ultimoAlunoLido, int *leituraSuperior, bool *leDireita) {
    
    // Ao ler o arquivo, nosso ponteiro se movimenta automaticamente,
    // com isso, devemos voltar o ponteiro uma posição,
    // porque a leitura superior caminha em direção ao início do vetor
    
    fseek(*arquivoLeituraEscritaSuperior, (*leituraSuperior - 1) * sizeof(Aluno), SEEK_SET);
    fread(ultimoAlunoLido, sizeof(Aluno), 1, *arquivoLeituraEscritaSuperior);
    (*leituraSuperior)--;
    *leDireita = false;
}

void inserirArea(Area *area, Aluno *ultimoAlunoLido, int *numeroAlunosArea) {
    
    // Insere o último aluno lido de forma ordenada na área
    insereItem(ultimoAlunoLido, area);
    *numeroAlunosArea = obterNumCelOcupadas(area);
}


void insereItem(Aluno *ultimoAlunoLido, Area *area){

    if(area->numeroAlunos >= TAMANHO_AREA)
        return;

    int indiceInsercao = 0;
    
    for(int i = 0; i < area->numeroAlunos; i++) {
        if(ultimoAlunoLido->nota > area->alunos[i].nota)
            indiceInsercao++;
            
        else
            break;
    }

    for(int i = area->numeroAlunos - 1; i >= indiceInsercao; i--){
        area->alunos[i + 1] = area->alunos[i];
    }

    area->alunos[indiceInsercao] = *ultimoAlunoLido;
    (area->numeroAlunos)++;

}

void escreveMinimo(FILE **arquivoEscritaInferior, Aluno aluno, int *escritaInferior){
    fwrite(&aluno, sizeof(Aluno), 1, *arquivoEscritaInferior);
    (*escritaInferior)++;
}

void escreveMaximo(FILE **arquivoLeituraEscritaSuperior, Aluno aluno, int *escritaSuperior) {
    fseek(*arquivoLeituraEscritaSuperior, (*escritaSuperior - 1) * sizeof(Aluno), SEEK_SET);
    fwrite(&aluno, sizeof(Aluno), 1, *arquivoLeituraEscritaSuperior);
    (*escritaSuperior)--;
}

void retiraMinimo(Area* area, Aluno* aluno, int* numeroAlunosArea){
    retiraPrimeiro(area, aluno);
    *numeroAlunosArea = obterNumCelOcupadas(area);
}


void retiraMaximo(Area* area, Aluno* aluno, int *numeroAlunosArea){
    retiraUltimo(area, aluno);
    *numeroAlunosArea = obterNumCelOcupadas(area);
}

void retiraPrimeiro(Area *area, Aluno *aluno){
    
    *aluno = area->alunos[0];

    for(int i = 0; i < area->numeroAlunos - 1; i++){
        area->alunos[i] = area->alunos[i + 1];
    }

    (area->numeroAlunos)--;
}

void retiraUltimo(Area *area, Aluno *aluno){
    *aluno = area->alunos[area->numeroAlunos - 1];
    (area->numeroAlunos)--;
}
