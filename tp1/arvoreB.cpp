#include <iostream>
#include <stdlib.h>

#include "arvoreB.h"
#include "struct.h"

using namespace std;

// Cria uma Árvore B em arquivo binário a partir de outro binário sequencial
bool fazArvoreB(char *nomeArquivoBinario, int quantidadeRegistros, Apontador arvoreB) {
    
    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    // Cria a Árvore B
    // Apontador arvoreB;
    iniciaArvoreB(arvoreB);

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

        // Constrói a Árvore B
        for(int i = 0; i < quantidadeItens; i++) { 
            insereArvoreB(pagina[i], &arvoreB);
        }

        paginaAtual++;
    }

    return true;
}

void iniciaArvoreB (Apontador raiz) {
    raiz = NULL;
}

bool insereArvoreB(Registro registro, Apontador *raiz) {

    cout << "registro a ser inserido: " << registro.chave << endl;
   
    bool CRESCEU; // true informa que a Árvore B cresceu pela raiz

    Registro registroRetorno; // registro da nova raiz
    Apontador apontadorRetorno; // filho à direita do registroRetorno

    // Insere o registro
    if(!insereRecursivo(registro, *raiz, &CRESCEU, &registroRetorno, &apontadorRetorno)) {
        return false; // se a inserção não for bem sucedida, retorna false
    }
    
    // Cria uma nova raiz quando a Árvore B cresce pela raiz
    if(CRESCEU) {
        cout << "CRESCEU" << endl;
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
        cout << "Registro já presente" << endl;
        *CRESCEU = false;
        return false;
    }

    if(registro.chave < paginaAtual->registros[i - 1].chave)
        i--;

    insereRecursivo(registro, paginaAtual->apontadores[i], CRESCEU, registroRetorno, apontadorRetorno);
    //------=-=--=-=-=
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

    return true;
}

bool insereNaPagina(Apontador pagina, Registro registro, Apontador apontadorDireito) {

    int quantidadeItens = pagina->itensInseridos;
    short naoAchou = (quantidadeItens > 0);

    while(naoAchou) {
        
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

bool pesquisaB(int chave, Registro *registro, Apontador pagina) {

    long indice = 1;

    if(pagina == NULL){
        cout << "Registro não está presente na árvore" << endl;
        return false;
    }

    while((indice < pagina->itensInseridos) && (chave > pagina->registros[indice - 1].chave)){
        indice++;
    }

    if(chave == pagina->registros[indice - 1].chave){
        *registro = pagina->registros[indice - 1];
        cout << "Registro encontrado" << endl;
        return true;
    }

    if(chave < pagina->registros[indice - 1].chave){
        pesquisaB(chave, registro, pagina->apontadores[indice - 1]);
    }
    else{
        pesquisaB(chave, registro, pagina->apontadores[indice]);
    }

    return false;
}

void imprimeArvoreB(Apontador pagina){

    if(pagina == NULL)
        return;

    for(int i = 0; i < 3; i++){
        cout << pagina->registros[i].chave << endl;
    }

}

void testeArvoreB(Apontador arvoreB){

    iniciaArvoreB(arvoreB);
    Registro registro;

    for(int i = 1; i <= 3; i++){
        registro.chave = i;
        insereArvoreB(registro, &arvoreB);

        // if(insereArvoreB(registro, &arvoreB))
        //     cout << "Inseriu." << endl;
        // else
        //     cout << "Não inseriu." << endl;
    }

    cout << "itens Pagina raiz: " << arvoreB->itensInseridos << endl;


    printTeste(arvoreB);
}

void printTeste(Apontador pagina){
    
    if(pagina == NULL)
        return;
    
    for(int i = 0; i < pagina->itensInseridos; i++){
        cout << pagina->registros[i].chave << endl;
    }

    for(int i = 0; i < pagina->itensInseridos + 1; i++){
        printTeste(pagina->apontadores[i]);
    }
}