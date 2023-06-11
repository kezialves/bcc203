#include <iostream>
#include <stdio.h>

#include "sequencial.h"
#include "struct.h"
#include "arquivo.h"

using namespace std;

bool pesquisaSequencial(Registro *registro, Argumento argumentos, char *nomeArquivoBinario, Registro *item) {

    int quantidadeItens;
    long deslocamento;
    Pagina pagina;
    
    int *tabela = fazTabela(nomeArquivoBinario, argumentos.quantidadeRegistros);
    FILE *arquivoBinario = NULL;
    
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) 
        return false;

    int indicePagina = 0;
    int tamanhoTabela = argumentos.quantidadeRegistros / ITENS_PAGINA;

    while(indicePagina < tamanhoTabela && tabela[indicePagina] <= argumentos.chave)
        indicePagina++;

    if(indicePagina == 0)
        return false;

    else {
        // A ultima página pode não estar completa
        if(indicePagina < tamanhoTabela)
            quantidadeItens = ITENS_PAGINA;
        
        else {
            fseek(arquivoBinario, 0, SEEK_END);
            quantidadeItens = (ftell(arquivoBinario) / sizeof(Registro)) % ITENS_PAGINA;
        }
        
        // Lê a página desejada do arquivo
        deslocamento = tabela[indicePagina] - 1 * ITENS_PAGINA * sizeof(Registro);
        fseek(arquivoBinario, deslocamento, SEEK_SET);
        fread(&pagina, sizeof(Registro), quantidadeItens, arquivoBinario);
        // pesquisa sequencial na página lida
        
        for(int i = 0; i < tamanhoTabela; i++) {
            if(pagina[i].chave == item->chave){
                *item = pagina[i];
                return true;
            }
        }
    }

    return true;
}

int *fazTabela(char *nomeArquivoBinario, int numeroRegistros) {

    FILE *arquivoBinario;

    // abreArquivo(arquivoBinario, nomeArquivoBinario);

    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Não foi possível abrir o arquivo binário\n";
    }

    Registro item;
    
    int tamanhoTabela = numeroRegistros / ITENS_PAGINA;
    int *tabela = (int*) malloc(tamanhoTabela * sizeof(int));
    int posicao = 0;

    // if(arquivoBinario == NULL)
    //     printf("NULO!!!!\n");

    // printf("Nome do arquivo: %s\n", nomeArquivoBinario);
    while(fread(&item, sizeof(Registro), 1, arquivoBinario) == 1) {
        // printf("-----Entrou no while\n");
        tabela[posicao] = item.chave;
        // printf("%d\n", tabela[posicao]);
        fseek(arquivoBinario, (ITENS_PAGINA - 1) * sizeof(item), SEEK_CUR);

        posicao++;
    }

    // printf("---------------\n");
    
    for(int i = 0; i < tamanhoTabela; i++) {
        printf("Poiscao %d - Chave: %d\n", i, tabela[i]);
    }

    fclose(arquivoBinario);
    return tabela;
}