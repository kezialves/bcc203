#include <iostream>
#include <stdio.h>

#include "arvoreBinaria.h"

using namespace std;

bool pesquisaBinaria(char* nomeArquivoArvoreBinaria, int quantidadeRegistros, int chave, Registro *registro){
    
    FILE *arquivoArvoreBinaria;

    // Verifica se foi possível abrir o arquivo da árvove binária
    // Caso contrário, retorna falso
    if((arquivoArvoreBinaria = fopen(nomeArquivoArvoreBinaria, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }
    
    PaginaNo pagina;
    int numeroPaginas = quantidadeRegistros / ITENS_PAGINA;
    int paginaAtual = 0, quantidadeItens;

    // Se a página não for a última, ela é completa,
    // então, a quantidade de itens é igual ao máximo de itens por página
    if(paginaAtual < numeroPaginas)
        quantidadeItens = ITENS_PAGINA;

    // Se for a última, ela pode não estar completa,
    // então, calcula a quantidade de itens
    else {
        fseek(arquivoArvoreBinaria, 0, SEEK_END);
        quantidadeItens = (ftell(arquivoArvoreBinaria) / sizeof(No)) % TAM_PAGINA_NO;
        fseek(arquivoArvoreBinaria, (numeroPaginas - 1) * ITENS_PAGINA * sizeof(No), SEEK_SET);
    }
    
    // Lê a página
    fread(&pagina, sizeof(No), quantidadeItens, arquivoArvoreBinaria);

    int indiceNoAtual = 0;
    bool ATUALIZA_PAGINA = false; // controla quando deve passar de uma página para a outra

    while(paginaAtual < numeroPaginas) {

        // Se a página não for a última, ela é completa,
        // então, a quantidade de itens é igual ao máximo de itens por página
        if(paginaAtual < numeroPaginas)
            quantidadeItens = ITENS_PAGINA;

        // Se for a última, ela pode não estar completa,
        // então, calcula a quantidade de itens
        else {
            fseek(arquivoArvoreBinaria, 0, SEEK_END);
            quantidadeItens = (ftell(arquivoArvoreBinaria) / sizeof(No)) % TAM_PAGINA_NO;
            fseek(arquivoArvoreBinaria, (numeroPaginas - 1) * ITENS_PAGINA * sizeof(No), SEEK_SET);
        }

        // Se a variável de controle estiver ligada, lê a próxima página e desliga
        if(ATUALIZA_PAGINA) {
            fread(&pagina, sizeof(No), quantidadeItens, arquivoArvoreBinaria);
            ATUALIZA_PAGINA = false;
        }

        // Define o nó atual como a raiz da árvore Binária ou o índice do último nó que a apontou
        No noAtual = pagina[indiceNoAtual];

        // Se encontra a chave pesquisada, salva o registro e retorna true
        if(noAtual.registro.chave == chave) {
            *registro = noAtual.registro;
            fclose(arquivoArvoreBinaria);
            return true;
        }

        // Se a chave pesquisada for maior do que a do nó atual,
        // caminha pela direita
        if(chave > noAtual.registro.chave) {

            // Se o apontador direito do nó não apontar para nenhum registro,
            // significa que a chave pesquisada não existe na árvore
            if(noAtual.direita == -1) {
                fclose(arquivoArvoreBinaria);
                return false;
            }
            
            // O apontador direito do nó aponta para algum registro
            else {
                // O nó apontado pelo nó atual vira o nó atual
                indiceNoAtual = noAtual.direita % TAM_PAGINA_NO; // % pois os índices reiniciam a cada página, indo de 0 a 50
                
                // Se o novo nó atual não estiver na página atual, aciona a mudança de página
                if(noAtual.direita > (paginaAtual + 1) * TAM_PAGINA_NO) {
                    ATUALIZA_PAGINA = true;
                    paginaAtual++;
                    continue;
                }
            }
        }
        
        // Se a chave pesquisada for menor do que a do nó atual,
        // caminha pela esquerda
        else {
            // Se o apontador esquerdo do nó não apontar para nenhum registro,
            // significa que a chave pesquisada não existe na árvore
            if(noAtual.esquerda == -1) {
                fclose(arquivoArvoreBinaria);
                return false;
            }
            
            // O apontador esquerdo do nó aponta para algum registro
            else {
                // O nó apontado pelo nó atual vira o nó atual
                indiceNoAtual = noAtual.esquerda % TAM_PAGINA_NO; // % pois os índices reiniciam a cada página, indo de 0 a 50
                
                // Se o novo nó atual não estiver na página atual, aciona a mudança de página
                if(noAtual.esquerda > (paginaAtual + 1)* TAM_PAGINA_NO) {
                    ATUALIZA_PAGINA = true;
                    paginaAtual++;
                    continue;
                }
            }
        }
    }

    return false;
}

bool fazArvoreBinaria(char *nomeArquivoBinario, char* nomeArquivoArvoreBinaria, int quantidadeRegistros) {

    FILE *arquivoBinario, *arquivoArvoreBinaria;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    // Verifica se foi possível abrir o arquivo árvore binária
    // Caso contrário, retorna falso
    if((arquivoArvoreBinaria = fopen(nomeArquivoArvoreBinaria, "wb")) == NULL) {
        cout << "Erro na abertura do arquivo da árvore binária.\n";
        return false;
    }

    Pagina pagina;
    int paginaAtual = 0, quantidadeItens;
    int numeroPaginas = quantidadeRegistros / ITENS_PAGINA;

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

        // Constrói a árvore binária
        for(int i = 0; i < quantidadeItens; i++) {     
            insereArvoreBinaria(nomeArquivoArvoreBinaria, pagina[i]);
        }

        paginaAtual++;
    }

    fclose(arquivoBinario);
    
    return true;
}

bool insereArvoreBinaria(char *nomeArquivoArvoreBinaria, Registro registro) {

    FILE *arquivoArvoreBinaria;

    // Verifica se foi possível abrir o arquivo da árvove binária
    // Caso contrário, informa erro
    if((arquivoArvoreBinaria = fopen(nomeArquivoArvoreBinaria, "rb+")) == NULL) {
        cout << "Erro na abertura do arquivo da árvore binária.\n";
        return false;
    }

    No novoNo = {registro, -1, -1};
    No noAtual;

    // Move o ponteiro do arquivo para o final e descobre a quantidade de registros
    fseek(arquivoArvoreBinaria, 0, SEEK_END);    
    int numeroRegistros = ftell(arquivoArvoreBinaria) / sizeof(No);
    
    // Se a árvore for vazia, insere o novo nó base
    if(numeroRegistros == 0) {
        fwrite(&novoNo, sizeof(No), 1, arquivoArvoreBinaria);
        fclose(arquivoArvoreBinaria);
        return true;
    }

    // Árvore não vazia

    // Reposiciona o ponteiro para o início do arquivo
    rewind(arquivoArvoreBinaria);

    while(fread(&noAtual, sizeof(No), 1, arquivoArvoreBinaria)) { // enquanto o fread ler um valor

        // O nó a ser inserido é menor do que o analisado
        // Caminha pela esquerda
        if(noAtual.registro.chave > novoNo.registro.chave) {
            // O nó analisado não aponta para nenhum nó pela esquerda
            if(noAtual.esquerda == -1) {
                // O nó analisado passa a apontar para onde o novo nó vai ser inserido
                noAtual.esquerda = numeroRegistros;
                // O ponteiro reposiciona para o início do nó atual e ele é reescrito
                fseek(arquivoArvoreBinaria, -1 * sizeof(No), SEEK_CUR);
                fwrite(&noAtual, sizeof(No), 1, arquivoArvoreBinaria);
                // O ponteiro reposiciona para onde o novo nó deve ser inserido e ele é escrito
                fseek(arquivoArvoreBinaria, 0, SEEK_END);
                fwrite(&novoNo, sizeof(No), 1, arquivoArvoreBinaria);
                // Fecha o arquivo e retorna true
                fclose(arquivoArvoreBinaria);     
                return true;
            }
            
            // O nó analisado aponta para outro nó pela esquerda
            // Reposiciona o ponteiro para esse outro nó e volta no início da repetição
            else {
                fseek(arquivoArvoreBinaria, (noAtual.esquerda) * sizeof(No), SEEK_SET);
                continue;
            }
        }

        // O nó a ser inserido é maior do que o analisado
        // Caminha pela direita
        else {
            // O nó analisado não aponta para nenhum nó pela direita
            if(noAtual.direita == -1) {
                // O nó analisado passa a apontar para onde o novo nó vai ser inserido
                noAtual.direita = numeroRegistros;
                // O ponteiro reposiciona para o início do nó atual e ele é reescrito
                fseek(arquivoArvoreBinaria, -1 * sizeof(No), SEEK_CUR);
                fwrite(&noAtual, sizeof(No), 1, arquivoArvoreBinaria);
                // O ponteiro reposiciona para onde o novo nó deve ser inserido e ele é escrito
                fseek(arquivoArvoreBinaria, 0, SEEK_END);
                fwrite(&novoNo, sizeof(No), 1, arquivoArvoreBinaria);
                // Fecha o arquivo e retorna true
                fclose(arquivoArvoreBinaria);        
                return true;
            }

            // O nó analisado aponta para outro nó pela direita
            // Reposiciona o ponteiro para esse outro nó e volta no início da repetição
            else {
                fseek(arquivoArvoreBinaria, (noAtual.direita) * sizeof(No), SEEK_SET);
                continue;
            }
        }
    }

    fclose(arquivoArvoreBinaria);
    return false;
}

void imprimeArvoreBinaria(char *nomeArquivoArvoreBinaria) {
    
    FILE * arquivoArvoreBinaria;
    No noAtual;

    // Verifica se foi possível abrir o arquivo da árvove binária
    // Caso contrário, retorna
    if((arquivoArvoreBinaria = fopen(nomeArquivoArvoreBinaria, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return;
    }    
    
    // int contador = 0;

    // Imprime a árvore e seus apontadores da esquerda e da direita
    while(fread(&noAtual, sizeof(No), 1, arquivoArvoreBinaria)) {
        cout << /*"Linha "<< contador << " |" <<*/ noAtual.esquerda << "|" << noAtual.registro.chave << "|" << noAtual.direita << endl;
        // contador++;
    }

    fclose(arquivoArvoreBinaria);
}