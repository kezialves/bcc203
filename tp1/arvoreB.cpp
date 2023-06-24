#include <iostream>
#include <stdlib.h>

#include "arvoreB.h"
#include "struct.h"

using namespace std;

// Cria uma Árvore B em arquivo binário a partir de outro binário sequencial
bool fazArvoreB(char *nomeArquivoBinario, int quantidadeRegistros) {
    
    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    // Cria a Árvore B
    Apontador arvoreB;
    iniciaArvoreB(arvoreB);

    // lê o arquivo binário base e insere os registros na árvore b
}

void iniciaArvoreB (Apontador raiz) {
    raiz = NULL;
}

bool insereArvoreB(Registro registro, Apontador *raiz) {

    bool CRESCEU; // true informa que a Árvore B cresceu pela raiz

    Registro registroRetorno; // registro da nova raiz
    Apontador apontadorRetorno; // filho à direita do registroRetorno

    // Insere o registro
    if(!insereRecursivo(registro, *raiz, &CRESCEU, &registroRetorno, &apontadorRetorno)) {
        return false; // se a inserção não for bem sucedida, retorna false
    }
    
    // Cria uma nova raiz quando a Árvore B cresce pela raiz
    if(CRESCEU) {
        // Aloca um apontador auxiliar que recebe as informações da nova raiz
        Apontador novaRaiz = (Apontador) malloc(sizeof(PaginaB));
        novaRaiz->itensInseridos = 1; // atualiza a quantidade de itens inseridos
        novaRaiz->registros[0] = registroRetorno; // o filho mais à esquerda recebe o registro retornado pela inserção
        novaRaiz->apontadores[1] = apontadorRetorno; // o apontador à direita recebe o filho à direita do registro
        novaRaiz->apontadores[0] = *raiz; // o apontador à esquerda recebe a sub-árvore
        *raiz = novaRaiz; // a árvore é atualizada com o endereço da nova raiz
    }

    return true;
}

bool insereRecursivo(Registro registro, Apontador paginaAtual, bool *CRESCEU, Registro *registroRetorno, Apontador *apontadorRetorno) {

    long i = 1, j;

    if(paginaAtual == NULL) {
        *CRESCEU = true;
        *registroRetorno = registro;
        *apontadorRetorno = NULL;
        return true;
    }

    // Inserção ordem crescente
    while(i < paginaAtual->itensInseridos && registro.chave > paginaAtual->registros[i - 1].chave) 
        i++;
    
    // Se o registro já foi inserido anteriormente, retorna false e encerra a recursão
    if(registro.chave == paginaAtual->registros[i - 1].chave) {
        *CRESCEU = false;
        return false;
    }

    if(registro.chave < paginaAtual->registros[i - 1].chave)
        i--;

    insereRecursivo(registro, paginaAtual->apontadores[i], CRESCEU, registroRetorno, apontadorRetorno);
    
    if(!*CRESCEU)
        return true; // false ou true ?????

    // Se a página tem espaço para inserir, insere e retorna sucesso
    if(paginaAtual->itensInseridos < ORDEM) {
        insereNaPagina(paginaAtual, *registroRetorno, *apontadorRetorno);
        *CRESCEU = false;
        return true;
    }

    // Overflow: Se não tiver espaço, a página precisa ser dividida
    Apontador novaPagina;
    novaPagina = (Apontador) malloc(sizeof(PaginaB));
    novaPagina->itensInseridos = 0;
    novaPagina->apontadores[0] = NULL;
    
    if(i < ORDEM + 1) {
        insereNaPagina(novaPagina, paginaAtual->registros[2 * ORDEM - 1], paginaAtual->apontadores[2 * ORDEM]);
        paginaAtual->itensInseridos--;
        insereNaPagina(paginaAtual, *registroRetorno, *apontadorRetorno);
    }

    else
        insereNaPagina(novaPagina, *registroRetorno, *apontadorRetorno);

    for(j = ORDEM + 2; j <= 2 * ORDEM; j++) {
        insereNaPagina(novaPagina, paginaAtual->registros[j - 1], paginaAtual->apontadores[j]);
    }

    paginaAtual->itensInseridos = ORDEM;
    novaPagina->apontadores[0] = paginaAtual->apontadores[ORDEM + 1];
    *registroRetorno = paginaAtual->registros[ORDEM];
    *apontadorRetorno = novaPagina;
}

bool insereNaPagina(Apontador pagina, Registro registro, Apontador apontadorDireito) {

    int quantidadeItens = pagina->itensInseridos;
    short naoAchou = (quantidadeItens > 0);
}