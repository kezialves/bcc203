#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "converteBin.h"
#include "aluno.h"

using namespace std;

bool textoParaBinario(char *nomeArquivoTexto, char *nomeArquivoBinario) {
    
    FILE *arquivoTexto, *arquivoBinario;
    Aluno aluno;
    
    // Verifica se foi possível abrir o arquivo texto
    // Caso contrário, retorna falso
    if((arquivoTexto = fopen(nomeArquivoTexto,"r")) == NULL) {
        cout << "Erro na abertura do arquivo texto.\n";
        return false;
    }

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "wb")) == NULL) {
        cout << "Erro na criação do arquivo binário.\n";
        return false;
    }
    
    // Lê os dados do arquivo texto e os escreve no arquivo binário
    while(fscanf(arquivoTexto, "%ld", &aluno.numeroInscricao) != EOF) {
        
        //Variáveis de controle para saber a posição da string e se chegamos no final, ao ler 2 espaços.
        int contadorEspaco = 0;
        int posicaoString = 0;
        
        char charLido;
        
        //fscanf para ler nota e estado, pois não há casos especiais para se tratar;
        fscanf(arquivoTexto, "%lf %s", &aluno.nota, aluno.estado);
        
        //consumir o espaço entre nome e estado, pois precisamos ler char por char.
        charLido = fgetc(arquivoTexto);

        while(contadorEspaco < 2) {
            
            charLido = fgetc(arquivoTexto);
            
            //if ternário para incrementar contador de espaço ou resetar caso seja um caractere normal
            (charLido == ' ') ? contadorEspaco++ : contadorEspaco = 0;
            
            if(contadorEspaco < 2){
                aluno.cidade[posicaoString] = charLido;
                posicaoString++;
            }
        }
        //ao terminar de ler a string, como lemos char por char devemos adicionar um \0 ao fim dela.
        aluno.cidade[posicaoString - 1] = '\0'; 

        //Como há vários espaços entre a cidade e o curso, optamos por um fscanf que consumirá apenas os espaços
        // e assim podemos ler o curso normalmente.
        fscanf(arquivoTexto, "%[ ]", aluno.curso);
        fgets(aluno.curso, STRING_TAM, arquivoTexto);

        //removendo o \n que vem junto do fgets
        aluno.curso[strlen(aluno.curso) - 2] = '\0';
        
        // Pega o tamanho do nome do curso para percorrer a string de trás para frente
        int tamanhoStringCurso = strlen(aluno.curso);

        // Percorre a string de trás para frente
        for(int i = tamanhoStringCurso - 1; i >= 0; i--) {

            // Se acabou os espaços, coloca um \0 na posição seguinte
            // para indicar que acabou o nome do curso
            if(aluno.curso[i] != ' '){
                aluno.curso[i + 1] = '\0';
                break;
            }
        }

        fwrite(&aluno, sizeof(Aluno), 1, arquivoBinario);
    }

    fclose(arquivoTexto);
    fclose(arquivoBinario);

    return true;
}

char *trocaExtensao(char *nomeArquivoTexto) {

    // Aloca uma struct para armazenar o nome do arquivo binário
    int tamanhoNome = strlen(nomeArquivoTexto);
    char *nomeArquivoBinario = (char*) malloc((tamanhoNome + 1) * sizeof(char));

    // Copia o nome do arquivo texto no binário e troca o "txt" por "bin"
    strcpy(nomeArquivoBinario, nomeArquivoTexto);
    nomeArquivoBinario[tamanhoNome - 3] = 'b';
    nomeArquivoBinario[tamanhoNome - 2] = 'i';
    nomeArquivoBinario[tamanhoNome - 1] = 'n';

    return nomeArquivoBinario;
}