#include <iostream>
#include <stdlib.h>
#include <chrono>

#include "arvoreB.h"
#include "struct.h"

using namespace std;
using namespace std::chrono;

bool pesquisaB(Argumentos argumentos, Registro *registro, Apontador pagina, Performance *performance) {

    long indice = 1;

    if(pagina == NULL) {
        return false;
    }

    // Caminha pela página enquanto houver itens e estes forem menores do que a chave pesquisada
    while((indice < pagina->itensInseridos) && (argumentos.chave > pagina->registros[indice - 1].chave)) {
        indice++;
        performance->comparacoes += 1;
    }

    if(argumentos.p == true)
        cout << "Chave pesquisada: " << pagina->registros[indice - 1].chave << endl;

    // Se a chave atual for a pesquisada, retorna sucesso
    
    performance->comparacoes += 1;

    if(argumentos.chave == pagina->registros[indice - 1].chave) {
        *registro = pagina->registros[indice - 1];
        return true;
    }

    // Se a chave pesquisada for menor do que a chave atual,
    // chama a pesquisa com a página anterior à da chave atual

    performance->comparacoes += 1;

    if(argumentos.chave < pagina->registros[indice - 1].chave) {
        if(pesquisaB(argumentos, registro, pagina->apontadores[indice - 1], performance)) {
            return true;
        }
    }

    // Se a chave pesquisada for maior do que a chave atual,
    // chama a pesquisa com a página seguinte à da chave atual

    else {
        if(pesquisaB(argumentos, registro, pagina->apontadores[indice], performance)) {
            return true;
        }
    }

    return false;
}

// Cria uma Árvore B em arquivo binário a partir de outro binário sequencial
bool fazArvoreB(char *nomeArquivoBinario, int quantidadeRegistros, Apontador *arvoreB, Performance *performance) {
    
    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    cout << "[Criando árvore B...]";
    auto start = high_resolution_clock::now();

    // Inicia a Árvore B
    iniciaArvoreB(*arvoreB);

    // Lê o arquivo binário base e insere os registros na Árvore B

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
        performance->transferencias += 1;

        // Constrói a Árvore B
        for(int i = 0; i < quantidadeItens; i++) { 
            insereArvoreB(pagina[i], arvoreB, performance);
        }

        paginaAtual++;
    }

    // Realiza a medição do tempo necessário para a criação da árvore em memória principal
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << " - Árvore criada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
    
    return true;
}

void iniciaArvoreB(Apontador raiz) {
    raiz = NULL;
}

bool insereArvoreB(Registro registro, Apontador *raiz, Performance *performance) {

    bool CRESCEU; // true informa que a Árvore B cresceu pela raiz

    Registro registroRetorno; // registro da nova raiz
    Apontador apontadorRetorno; // filho à direita do registroRetorno

    // Insere o registro
    if(!insereRecursivo(registro, *raiz, &CRESCEU, &registroRetorno, &apontadorRetorno, performance)) {
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

bool insereRecursivo(Registro registro, Apontador paginaAtual, bool *CRESCEU, Registro *registroRetorno, Apontador *apontadorRetorno, Performance *performance) {

    long i = 1, j;

    if(paginaAtual == NULL) {
        *CRESCEU = true;
        *registroRetorno = registro;
        *apontadorRetorno = NULL;
        return true;
    }

    // Caminha pela página enquanto houver itens e estes forem menores do que a chave pesquisada
    while(i < paginaAtual->itensInseridos && registro.chave > paginaAtual->registros[i - 1].chave) {
        i++;
        performance->comparacoes += 1;
    }

    // Se o registro já foi inserido anteriormente, retorna false e encerra a recursão

    performance->comparacoes += 1;
    
    if(registro.chave == paginaAtual->registros[i - 1].chave) {
        // cout << "Registro já presente" << endl;
        *CRESCEU = false;
        return false;
    }

    // Se a chave pesquisada for menor do que a chave atual,
    // diminui 1 do índice para chamar a página anterior à da chave atual.
    // Caso contrário, o índice permanece igual para chamar a página seguinte à da chave atual.

    performance->comparacoes += 1;

    if(registro.chave < paginaAtual->registros[i - 1].chave)
        i--;

    insereRecursivo(registro, paginaAtual->apontadores[i], CRESCEU, registroRetorno, apontadorRetorno, performance);

    if(!*CRESCEU)
        return true;

    // Se a página tem espaço para inserir, insere e retorna sucesso
    if(paginaAtual->itensInseridos < 2 * ORDEM) {
        insereNaPagina(paginaAtual, *registroRetorno, *apontadorRetorno, performance);
        *CRESCEU = false;
        return true;
    }

    // Overflow: Se não tiver espaço, a página precisa ser dividida
    Apontador novaPagina;
    novaPagina = (Apontador) malloc(sizeof(PaginaB));
    novaPagina->itensInseridos = 0;
    novaPagina->apontadores[0] = NULL;

    // Verifica a posição do elemento a ser inserido na página
    // Caso ele deva ficar à esquerda do índice do meio, deixamos ele na página atual
    if(i < ORDEM + 1) {
        insereNaPagina(novaPagina, paginaAtual->registros[2 * ORDEM - 1], paginaAtual->apontadores[2 * ORDEM], performance);
        paginaAtual->itensInseridos--;
        insereNaPagina(paginaAtual, *registroRetorno, *apontadorRetorno, performance);
    }

    // Caso ele deva ficar à direita, ou seja o índice do meio, ele irá ficar na página nova
    else
        insereNaPagina(novaPagina, *registroRetorno, *apontadorRetorno, performance);

    // A página nova deve conter os elementos da página atual a partir da posição 2M
    // Joga os elementos da posição 2M até a posição final para a página nova
    for(j = ORDEM + 2; j <= 2 * ORDEM; j++) {
        insereNaPagina(novaPagina, paginaAtual->registros[j - 1], paginaAtual->apontadores[j], performance);
    }

    // Como houve a criação da página nova, pela falta de espaço,
    // a página atual passa a ter a ordem mínima para existência
    paginaAtual->itensInseridos = ORDEM;
    novaPagina->apontadores[0] = paginaAtual->apontadores[ORDEM + 1];
    *registroRetorno = paginaAtual->registros[ORDEM];
    *apontadorRetorno = novaPagina;

    return true;
}

bool insereNaPagina(Apontador pagina, Registro registro, Apontador apontadorDireito, Performance *performance) {

    int quantidadeItens = pagina->itensInseridos;
    short naoAchou = (quantidadeItens > 0);

    // Percorre a página até encontrar a posição que o item deve ser inserido
    while(naoAchou) {
        
        performance->comparacoes += 1;

        if(registro.chave >= pagina->registros[quantidadeItens - 1].chave) {
            naoAchou = false;
            break;
        }

        pagina->registros[quantidadeItens] = pagina->registros[quantidadeItens - 1];
        pagina->apontadores[quantidadeItens + 1] = pagina->apontadores[quantidadeItens];
        quantidadeItens--;
        
        if(quantidadeItens < 1)
            naoAchou = false;
    }

    pagina->registros[quantidadeItens] = registro;
    pagina->apontadores[quantidadeItens + 1] = apontadorDireito;
    pagina->itensInseridos++;

    return true;
}