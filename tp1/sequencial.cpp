#include <iostream>
#include <stdio.h>

#include "sequencial.h"
#include "struct.h"
#include "arquivo.h"

using namespace std;

bool pesquisaSequencial(Argumentos argumentos, char *nomeArquivoBinario, Registro *item) {

    Pagina pagina;
    int quantidadeItens;
    long deslocamento;
    
    int *tabela = fazTabela(nomeArquivoBinario, argumentos.quantidadeRegistros);
    FILE *arquivoBinario = NULL;
    
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) 
        return false;

    int indicePagina = 0;
    int tamanhoTabela = argumentos.quantidadeRegistros / ITENS_PAGINA;
    cout << "tamtabela: " << tamanhoTabela << endl;

    while(indicePagina < tamanhoTabela && tabela[indicePagina] <= argumentos.chave)
        indicePagina++;

    cout << "IndicePagina = " << indicePagina << endl;

    if(indicePagina == 0)
        return false;

    else {
        // A ultima página pode não estar completa
        cout << "Indice Página : " << indicePagina << " < " << tamanhoTabela << "Tamanho Tabela" << endl;
        if(indicePagina <= tamanhoTabela)
            quantidadeItens = ITENS_PAGINA;
        
        else {
            fseek(arquivoBinario, 0, SEEK_END);
            quantidadeItens = (ftell(arquivoBinario) / sizeof(Registro)) % ITENS_PAGINA;
        }

        cout << "tamanho ftell : " << ftell(arquivoBinario) << "/" << sizeof(Registro) << "%" << ITENS_PAGINA << endl;

        cout << "(1)quantidade itens: " << quantidadeItens << endl;

        // Lê a página desejada do arquivo
        deslocamento = (indicePagina - 1) * ITENS_PAGINA * sizeof(Registro);
        // ((51) - 1) * 50 * Sizeof
        cout << "tabela[indicePagina - 1] = " << tabela[indicePagina - 1] << endl;
        cout << "Itens Página :" << ITENS_PAGINA << endl;

        fseek(arquivoBinario, deslocamento, SEEK_SET);
        fread(&pagina, sizeof(Registro), quantidadeItens, arquivoBinario);
        
        // Pesquisa sequencial na página lida
        cout << "(2)quantidade itens: " << quantidadeItens << endl;
        
        for(int i = 0; i < quantidadeItens; i++) {
            cout << pagina[i].chave << endl;
            if(pagina[i].chave == argumentos.chave) {
                *item = pagina[i];
                return true;
            }
        }

        return false;
    }
}

int *fazTabela(char *nomeArquivoBinario, int numeroRegistros) {

    FILE *arquivoBinario;

    // abreArquivo(arquivoBinario, nomeArquivoBinario);

    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Não foi possível abrir o arquivo binário.\n";
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
    
    // for(int i = 0; i < tamanhoTabela; i++) {
    //     printf("Poiscao %d - Chave: %d\n", i, tabela[i]);
    // }

    cout << "pos: " << posicao << endl;
    fclose(arquivoBinario);
    return tabela;
}