#include <iostream>
#include <stdlib.h>

#include "arvoreBest.h"
#include "struct.h"

using namespace std;

bool pesquisaBest(int chave, Registro *registro, ApontadorBest *pagina) {

    int i;
    
    if((*pagina)->tipoPagina == Interna) {

        i = 1;

        while(i < (*pagina)->UU.U0.quantidadeChaves && chave > (*pagina)->UU.U0.chaves[i-1])
            i++;
        
        if(chave <= (*pagina)->UU.U0.chaves[i-1]) {
            if(pesquisaBest(chave, registro, &((*pagina)->UU.U0.apontadores[i-1])))
                return true;
        }

        else {
            if(pesquisaBest(chave, registro, &((*pagina)->UU.U0.apontadores[i])))
                return true;
        }
            
        return false;
    }

    i = 1;
    
    while(i < (*pagina)->UU.U1.quantidadeRegistros && chave > (*pagina)->UU.U1.registros[i-1].chave)
        i++;
    
    if(chave == (*pagina)->UU.U1.registros[i-1].chave) {
        *registro = (*pagina)->UU.U1.registros[i-1];
        return true;
    }

    return false;
}

bool fazArvoreBest(char *nomeArquivoBinario, int quantidadeRegistros, ApontadorBest *arvoreBest) {
    
    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    // Inicia a Árvore B*
    iniciaArvoreBest(arvoreBest);

    // Lê o arquivo binário base e insere os registros na Árvore B*

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

        // Constrói a Árvore B*
        for(int i = 0; i < quantidadeItens; i++) { 
            insereArvoreBest(pagina[i], arvoreBest);
        }

        paginaAtual++;
    }

    return true;
}

void iniciaArvoreBest(ApontadorBest *raiz) {
    *raiz = (ApontadorBest) malloc(sizeof(PaginaBest));
    (*raiz)->tipoPagina = Externa;
    (*raiz)->UU.U1.quantidadeRegistros = 0;
    
}

bool insereArvoreBest(Registro registro, ApontadorBest *raiz) {

    bool CRESCEU; // true informa que a Árvore B* cresceu pela raiz

    Registro registroRetorno; // registro da nova raiz
    ApontadorBest apontadorRetorno; // filho à direita do registroRetorno

    // Insere o registro
    if(!insereRecursivo(registro, *raiz, &CRESCEU, &registroRetorno, &apontadorRetorno)) {
        return false; // se a inserção não for bem sucedida, retorna false
    }
    
    // Cria uma nova raiz quando a Árvore B* cresce pela raiz
    if(CRESCEU) {
        // Aloca um apontador auxiliar que recebe as informações da nova raiz
        ApontadorBest novaRaiz = (ApontadorBest) malloc(sizeof(PaginaBest));
        novaRaiz->tipoPagina = Interna;
        novaRaiz->UU.U0.quantidadeChaves = 1; // atualiza a quantidade de chaves
        novaRaiz->UU.U0.chaves[0] = registroRetorno.chave; // o filho mais à esquerda recebe a chave do registro retornado pela inserção
        novaRaiz->UU.U0.apontadores[1] = apontadorRetorno; // o apontador à direita recebe o filho à direita do registro
        novaRaiz->UU.U0.apontadores[0] = *raiz; // o apontador à esquerda recebe a sub-árvore
        *raiz = novaRaiz; // a árvore é atualizada com o endereço da nova raiz
    }

    return true;
}

bool insereRecursivo(Registro registro, ApontadorBest paginaAtual, bool *CRESCEU, Registro *registroRetorno, ApontadorBest *apontadorRetorno) {

    int i = 1, j;
    ApontadorBest novaPagina;

    // Chegou numa página folha
    if(paginaAtual->tipoPagina == Externa) {

        // Não tem espaço na página folha, precisa criar uma nova e dividir
        if(paginaAtual->UU.U1.quantidadeRegistros == MM2) {

            // Cria e inicializa uma página folha nova
            novaPagina = (ApontadorBest) malloc(sizeof(PaginaBest));
            novaPagina->tipoPagina = Externa;
            novaPagina->UU.U1.quantidadeRegistros = 0;

            // Divide a página
            if(i < M + 1){
                // Joga o último elemento da página atual para a página nova, abrindo espaço
                insereNaPaginaExterna(novaPagina, paginaAtual->UU.U1.registros[MM2 - 1]);
                paginaAtual->UU.U1.quantidadeRegistros--; // atualiza a quantidade de registros da página atual
                // Insere o registro na página atual
                insereNaPaginaExterna(paginaAtual, registro);
            }
            
            else
                insereNaPaginaExterna(novaPagina, registro);        

            // A página nova deve conter os elementos da página atual a partir da posição 2M
            // Joga os elementos da posição 2M até a posição final para a página nova
            for(j = M + 2; j <= MM2; j++) {
                insereNaPaginaExterna(novaPagina, paginaAtual->UU.U1.registros[j - 1]);
            }

            paginaAtual->UU.U1.quantidadeRegistros = M + 1; // atualiza a quantidade de registros da página atual
            
            // Atualiza os retornos e aciona a CRESCEU
            // Uma vez que a página folha foi dividida, a página nó recebe um novo elemento
            *registroRetorno = paginaAtual->UU.U1.registros[M];
            *apontadorRetorno = novaPagina;
            *CRESCEU = true;

            return true;
        }

        // Tem espaço na página folha, insere
        else {
            insereNaPaginaExterna(paginaAtual, registro);
            *CRESCEU = false;
            return true;
        }
    }

    // Está numa página nó
    else {   
        // Caminha pelas páginas nó até achar uma página com a chave pesquisada
        while(i < paginaAtual->UU.U0.quantidadeChaves && registro.chave > paginaAtual->UU.U0.chaves[i - 1]) 
            i++;
        
        // Se a chave já está presente e é uma página folha, retorna false e encerra a recursão
        // A chave estar em uma página nó não quer dizer que ela existe como registro em uma página folha
        // Mas, dessa forma, funciona neste caso
        if(registro.chave == paginaAtual->UU.U0.chaves[i - 1]) {
            *CRESCEU = false;
            return false;
        }

        //
        if(registro.chave < paginaAtual->UU.U0.chaves[i - 1])
            i--;

        insereRecursivo(registro, paginaAtual->UU.U0.apontadores[i], CRESCEU, registroRetorno, apontadorRetorno);


        if(!*CRESCEU) 
            return true;
        
        if(paginaAtual->UU.U0.quantidadeChaves < MM) {
            insereNaPaginaInterna(paginaAtual, registroRetorno->chave, *apontadorRetorno);
            *CRESCEU = false;
            return true;
        }

        novaPagina = (ApontadorBest) malloc(sizeof(PaginaBest));
        novaPagina->tipoPagina = Interna;
        novaPagina->UU.U0.quantidadeChaves = 0;
        novaPagina->UU.U0.apontadores[0] = NULL;

    //dsdsdsdsdsdsd
        if(i < M + 1){
            insereNaPaginaInterna(novaPagina, paginaAtual->UU.U0.chaves[MM - 1], paginaAtual->UU.U0.apontadores[MM]);
            paginaAtual->UU.U0.quantidadeChaves--;
            insereNaPaginaInterna(paginaAtual, registroRetorno->chave, *apontadorRetorno);
        }

        else{
            insereNaPaginaInterna(novaPagina, registroRetorno->chave, *apontadorRetorno);
        }

        for(j = M + 2; j <= MM; j++)
            insereNaPaginaInterna(novaPagina, paginaAtual->UU.U0.chaves[j-1], paginaAtual->UU.U0.apontadores[j]);

        paginaAtual->UU.U0.quantidadeChaves = M;
        novaPagina->UU.U0.apontadores[0] = paginaAtual->UU.U0.apontadores[M + 1];
        registroRetorno->chave = paginaAtual->UU.U0.chaves[M];
        *apontadorRetorno = novaPagina;
    }

    return true;
}

bool insereNaPaginaInterna(ApontadorBest pagina, int chave, ApontadorBest apontadorDireito) {

    int quantidadeItens = pagina->UU.U0.quantidadeChaves;
    bool naoAchou = (quantidadeItens > 0);
    
    while(naoAchou) {

        if(chave >= pagina->UU.U0.chaves[quantidadeItens - 1]) {
            naoAchou = false;
            break;
        }

        pagina->UU.U0.chaves[quantidadeItens] = pagina->UU.U0.chaves[quantidadeItens - 1];
        pagina->UU.U0.apontadores[quantidadeItens + 1] = pagina->UU.U0.apontadores[quantidadeItens];
        quantidadeItens--;

        if(quantidadeItens < 1)
            naoAchou = false;
    }

    pagina->UU.U0.chaves[quantidadeItens] = chave;
    pagina->UU.U0.apontadores[quantidadeItens + 1] = apontadorDireito;
    pagina->UU.U0.quantidadeChaves++;

    return true;
}

bool insereNaPaginaExterna(ApontadorBest pagina, Registro registro) {

    int quantidadeItens = pagina->UU.U1.quantidadeRegistros;
    short naoAchou = (quantidadeItens > 0);

    while(naoAchou) {

        if(registro.chave >= pagina->UU.U1.registros[quantidadeItens-1].chave) {
            naoAchou = false;
            break;
        }

        pagina->UU.U1.registros[quantidadeItens] = pagina->UU.U1.registros[quantidadeItens - 1];
        quantidadeItens--;

        if(quantidadeItens < 1) 
            naoAchou = false;
    }

    pagina->UU.U1.registros[quantidadeItens] = registro;
    pagina->UU.U1.quantidadeRegistros++;

    return true;
}

void imprimeArvoreBest(ApontadorBest pagina) {

    for(int i = 0; i < 3; i++) {
        cout << pagina->UU.U0.chaves[i] << endl;
    }
}