#include <iostream>
#include <stdio.h>
#include <chrono>

#include "sequencial.h"
#include "struct.h"

using namespace std;
using namespace std::chrono;

bool pesquisaSequencial(Argumentos argumentos, char *nomeArquivoBinario, Registro *item, Performance *performance) {

    Pagina pagina;
    int quantidadeItens;
    long deslocamento;
    
    // Cria a tabela e um ponteiro para o arquivo

    cout << "[Criando tabela de páginas...]";
    auto start = high_resolution_clock::now();
    int *tabela = fazTabela(nomeArquivoBinario, argumentos.quantidadeRegistros, performance);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << " - Tabela criada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;

    cout << "[Realizando pesquisa sequencial...]";
    auto start2 = high_resolution_clock::now();

    FILE *arquivoBinario = NULL;
    
    // Se a abertura do arquivo retornar um apontador nulo, libera a tabela e retorna
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        free(tabela);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start2);
        cout << " - Não foi possível realizar pesquisa sequencial: ( " << duration.count() << " nanosegundos ) " << endl;
        return false;
    }

    int indicePagina = 0;
    int tamanhoTabela = argumentos.quantidadeRegistros / ITENS_PAGINA;

    // Descobre o método de ordenação do arquivo
    switch(argumentos.tipoOrdenacao) {

        // Ordenado ascendentemente (ordem crescente)
        case 1:
            while(indicePagina < tamanhoTabela && tabela[indicePagina] <= argumentos.chave)
                indicePagina++;
            break;

        // Ordenado descendentemente (ordem decrescente)
        case 2:
            while(indicePagina < tamanhoTabela && tabela[indicePagina] >= argumentos.chave)
                indicePagina++;
            break;

        // Arquivo aleatório
        case 3:
            // No arquivo aleatório, não tem como descobrir o índice da página 
            break;
    }

    if(indicePagina == 0 && argumentos.tipoOrdenacao != 3) {
        free(tabela);
        fclose(arquivoBinario);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start2);
        cout << " - Pesquisa sequencial realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
        return false;
    }

    // Se o arquivo for ordenado, lê a página em que a chave deve estar
    else if(argumentos.tipoOrdenacao != 3) {

        // Se a página não for a última, ela é completa,
        // então, a quantidade de itens é igual ao máximo de itens por página
        if(indicePagina <= tamanhoTabela)
            quantidadeItens = ITENS_PAGINA;
        
        // Se for a última, ela pode não estar completa,
        // então, calcula a quantidade de itens
        else {
            fseek(arquivoBinario, 0, SEEK_END);
            quantidadeItens = (ftell(arquivoBinario) / sizeof(Registro)) % ITENS_PAGINA;
        }

        // Lê a página desejada do arquivo
        deslocamento = (indicePagina - 1) * ITENS_PAGINA * sizeof(Registro);
        fseek(arquivoBinario, deslocamento, SEEK_SET);
        fread(&pagina, sizeof(Registro), quantidadeItens, arquivoBinario);
        performance->transferencias += 1;
        fclose(arquivoBinario);
        
        // Pesquisa sequencial na página lida
        for(int i = 0; i < quantidadeItens; i++) { 

            performance->comparacoes += 1;

            if(argumentos.p)
                cout << "Chave pesquisada: " << pagina[i].chave << endl;

            if(pagina[i].chave == argumentos.chave) {
                *item = pagina[i];
                free(tabela);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start2);
                cout << " - Pesquisa sequencial realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
                return true;
            }
        }

        free(tabela);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start2);
        cout << " - Pesquisa sequencial realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
        return false;
    }

    // Arquivo desordenado
    else {
        
        int numeroPaginas = argumentos.quantidadeRegistros / ITENS_PAGINA;
        int paginaAtual = 0;

        // Como não tem como saber em que página o registro deveria estar,
        // pesquisa no arquivo inteiro
        while(paginaAtual < numeroPaginas) {

            // Se a página não for a última, ela é completa,
            // então, a quantidade de itens é igual ao máximo de itens por página
            if(paginaAtual < numeroPaginas)
                quantidadeItens = ITENS_PAGINA;

            // Se for a última, ela pode não estar completa,
            // então, calcula a quantidade de itens
            else {
                fseek(arquivoBinario, 0, SEEK_END);
                quantidadeItens = (ftell(arquivoBinario) / sizeof(Registro)) % ITENS_PAGINA;
                fseek(arquivoBinario, (numeroPaginas - 1) * ITENS_PAGINA * sizeof(Registro), SEEK_SET);
            }
            
            // Lê a página
            fread(&pagina, sizeof(Registro), quantidadeItens, arquivoBinario);
            performance->transferencias += 1;
              
            // Pesquisa sequencial na página lida
            for(int i = 0; i < quantidadeItens; i++) {

                performance->comparacoes += 1;

                if(argumentos.p)
                    cout << "Chave pesquisada: " << pagina[i].chave << endl;    

                if(pagina[i].chave == argumentos.chave) {
                    *item = pagina[i];
                    free(tabela);
                    fclose(arquivoBinario);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start2);
                    cout << " - Pesquisa sequencial realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
                    return true;
                }
            }
            
            paginaAtual++;
        }

        fclose(arquivoBinario);
        free(tabela);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start2);
        cout << " - Pesquisa sequencial realizada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
        return false;
    }

    free(tabela);
    return false;
}

int *fazTabela(char *nomeArquivoBinario, int numeroRegistros, Performance *performance) {

    FILE *arquivoBinario;

    // Tenta abrir o arquivo binário e retorna erro caso não consiga
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Não foi possível abrir o arquivo binário.\n";
    }

    Registro item;
    
    // Aloca a tabela
    int tamanhoTabela = numeroRegistros / ITENS_PAGINA;
    int *tabela = (int*) malloc(tamanhoTabela * sizeof(int));
    int posicao = 0;

    // Preenche a tabela
    while(fread(&item, sizeof(Registro), 1, arquivoBinario) == 1 && posicao < tamanhoTabela) { 
        tabela[posicao] = item.chave; 
        fseek(arquivoBinario, (ITENS_PAGINA - 1) * sizeof(item), SEEK_CUR);

        performance->transferencias += 1;

        posicao++;
    }

    fclose(arquivoBinario);
    return tabela;
}