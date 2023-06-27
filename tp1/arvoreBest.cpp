#include <iostream>
#include <stdlib.h>
#include <chrono>

#include "arvoreBest.h"
#include "struct.h"

using namespace std;
using namespace std::chrono;


bool pesquisaBest(Argumentos argumentos, Registro *registro, ApontadorBest *pagina, Performance *performance) {

    int i;
    
    // Se a página for interna, não pode ter registros, apenas índices
    // Por isso, devemos percorrer as páginas nós até achar a
    // página folha em que o item deve estar inserido
    
    if((*pagina)->tipoPagina == Interna) {

        i = 1;

        // Caminha pela página enquanto houver itens e estes forem menores do que a chave pesquisada
        while(i < (*pagina)->UU.U0.quantidadeChaves && argumentos.chave > (*pagina)->UU.U0.chaves[i-1]) {
            i++;
            performance->comparacoes += 1;
        }
        
        if(argumentos.p == true)
            cout << "Chave pesquisada: " << (*pagina)->UU.U0.chaves[i-1] << endl;

        // Se a chave pesquisada for menor ou igual à chave atual,
        // chama a pesquisa com a página anterior à da chave atual

        performance->comparacoes += 1;

        if(argumentos.chave <= (*pagina)->UU.U0.chaves[i-1]) {
            if(pesquisaBest(argumentos, registro, &((*pagina)->UU.U0.apontadores[i-1]), performance))
                return true;
        }

        // Se a chave pesquisada for maior do que a chave atual,
        // chama a pesquisa com a página seguinte à da chave atual

        else {
            if(pesquisaBest(argumentos, registro, &((*pagina)->UU.U0.apontadores[i]), performance))
                return true;
        }
            
        return false;
    }

    i = 1;
    
    // Caso a página seja uma folha, página de registros, devemos verificar a posição esperada do registro
    while(i < (*pagina)->UU.U1.quantidadeRegistros && argumentos.chave > (*pagina)->UU.U1.registros[i-1].chave) {
        i++;
        performance->comparacoes += 1;
    }

    // cout << "Chave pesquisada: " << (*pagina)->UU.U1.registros[i-1].chave << endl;

    performance->comparacoes += 1;

    // Se o registro estiver na posição esperada, retornamos a busca e ela foi finalizada
    if(argumentos.chave == (*pagina)->UU.U1.registros[i-1].chave) {
        *registro = (*pagina)->UU.U1.registros[i-1];
        return true;
    }

    return false;
}


bool fazArvoreBest(char *nomeArquivoBinario, int quantidadeRegistros, ApontadorBest *arvoreBest, Performance *performance) {
    
    FILE *arquivoBinario;

    // Verifica se foi possível abrir o arquivo binário
    // Caso contrário, retorna falso
    if((arquivoBinario = fopen(nomeArquivoBinario, "rb")) == NULL) {
        cout << "Erro na abertura do arquivo binário.\n";
        return false;
    }

    cout << "[Criando árvore B*...]";
    auto start = high_resolution_clock::now();

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
        performance->transferencias += 1;

        // Constrói a Árvore B*
        for(int i = 0; i < quantidadeItens; i++) { 
            insereArvoreBest(pagina[i], arvoreBest, performance);
        }

        paginaAtual++;
    }

    // Realiza a medição do tempo necessário para a criação da árvore em memória principal
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << " - Árvore criada com sucesso: ( " << duration.count() << " nanosegundos ) " << endl;
    
    return true;
}

// Inicialmente, a Árvore B* possui apenas uma página folha, sua raiz é externa
// A primeira página interna só surge quando há overflow na raiz
// Por isso, iniciamos a Árvore B* assim
void iniciaArvoreBest(ApontadorBest *raiz) {
    *raiz = (ApontadorBest) malloc(sizeof(PaginaBest));
    (*raiz)->tipoPagina = Externa;
    (*raiz)->UU.U1.quantidadeRegistros = 0;
}

bool insereArvoreBest(Registro registro, ApontadorBest *raiz, Performance *performance) {

    bool CRESCEU; // true informa que a Árvore B* cresceu pela raiz

    Registro registroRetorno; // registro da nova raiz
    ApontadorBest apontadorRetorno; // filho à direita do registroRetorno

    // Insere o registro
    if(!insereRecursivo(registro, *raiz, &CRESCEU, &registroRetorno, &apontadorRetorno, performance)) {
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

bool insereRecursivo(Registro registro, ApontadorBest paginaAtual, bool *CRESCEU, Registro *registroRetorno, ApontadorBest *apontadorRetorno, Performance *performance) {

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
            if(i < M + 1) {
                // Joga o último elemento da página atual para a página nova, abrindo espaço
                insereNaPaginaExterna(novaPagina, paginaAtual->UU.U1.registros[MM2 - 1], performance);
                paginaAtual->UU.U1.quantidadeRegistros--; // atualiza a quantidade de registros da página atual
                // Insere o registro na página atual
                insereNaPaginaExterna(paginaAtual, registro, performance);
            }
            
            else
                insereNaPaginaExterna(novaPagina, registro, performance);        

            // A página nova deve conter os elementos da página atual a partir da posição 2M
            // Joga os elementos da posição 2M até a posição final para a página nova
            for(j = M + 2; j <= MM2; j++) {
                insereNaPaginaExterna(novaPagina, paginaAtual->UU.U1.registros[j - 1], performance);
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
            insereNaPaginaExterna(paginaAtual, registro, performance);
            *CRESCEU = false;
            return true;
        }
    }

    // Está numa página nó
    else {   
        // Caminha pelas páginas nó até achar uma página com a chave pesquisada
        while(i < paginaAtual->UU.U0.quantidadeChaves && registro.chave > paginaAtual->UU.U0.chaves[i - 1]) {
            i++;
            performance->comparacoes += 1;
        }

        // Se a chave já está presente e é uma página folha, retorna false e encerra a recursão
        // A chave estar em uma página nó não quer dizer que ela existe como registro em uma página folha
        // Mas, dessa forma, funciona neste caso

        performance->comparacoes += 1;

        if(registro.chave == paginaAtual->UU.U0.chaves[i - 1]) {
            *CRESCEU = false;
            return false;
        }

        // Se a chave pesquisada for menor do que a chave atual,
        // diminui 1 do índice para chamar a página anterior à da chave atual.
        // Caso contrário, o índice permanece igual para chamar a página seguinte à da chave atual.

        performance->comparacoes += 1;

        if(registro.chave < paginaAtual->UU.U0.chaves[i - 1])
            i--;

        insereRecursivo(registro, paginaAtual->UU.U0.apontadores[i], CRESCEU, registroRetorno, apontadorRetorno, performance);

        if(!*CRESCEU) 
            return true;
        
        // Se a página tem espaço para inserir, insere e retorna sucesso
        if(paginaAtual->UU.U0.quantidadeChaves < MM) {
            insereNaPaginaInterna(paginaAtual, registroRetorno->chave, *apontadorRetorno, performance);
            *CRESCEU = false;
            return true;
        }

        // Overflow: Se não tiver espaço, a página precisa ser dividida
        novaPagina = (ApontadorBest) malloc(sizeof(PaginaBest));
        novaPagina->tipoPagina = Interna;
        novaPagina->UU.U0.quantidadeChaves = 0;
        novaPagina->UU.U0.apontadores[0] = NULL;

        // Verifica a posição do elemento a ser inserido na página
        // Caso ele deva ficar à esquerda do índice do meio, deixamos ele na página atual
        if(i < M + 1) {
            insereNaPaginaInterna(novaPagina, paginaAtual->UU.U0.chaves[MM - 1], paginaAtual->UU.U0.apontadores[MM], performance);
            paginaAtual->UU.U0.quantidadeChaves--;
            insereNaPaginaInterna(paginaAtual, registroRetorno->chave, *apontadorRetorno, performance);
        }
        
        // Caso ele deva ficar à direita, ou seja o índice do meio, ele irá ficar na página nova
        else {
            insereNaPaginaInterna(novaPagina, registroRetorno->chave, *apontadorRetorno, performance);
        }
        
        // Arrasta a metade final dos elementos da página atual para a página nova
        for(j = M + 2; j <= MM; j++)
            insereNaPaginaInterna(novaPagina, paginaAtual->UU.U0.chaves[j-1], paginaAtual->UU.U0.apontadores[j], performance);

        // Como houve a criação da página nova, pela falta de espaço,
        // a página atual passa a ter a ordem mínima para existência
        paginaAtual->UU.U0.quantidadeChaves = M;
        novaPagina->UU.U0.apontadores[0] = paginaAtual->UU.U0.apontadores[M + 1];
        registroRetorno->chave = paginaAtual->UU.U0.chaves[M];
        *apontadorRetorno = novaPagina;
    }

    return true;
}

bool insereNaPaginaInterna(ApontadorBest pagina, int chave, ApontadorBest apontadorDireito, Performance *performance) {

    int quantidadeItens = pagina->UU.U0.quantidadeChaves;
    bool naoAchou = (quantidadeItens > 0);
    
    while(naoAchou) {

        performance->comparacoes += 1;

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

bool insereNaPaginaExterna(ApontadorBest pagina, Registro registro, Performance *performance) {

    int quantidadeItens = pagina->UU.U1.quantidadeRegistros;
    short naoAchou = (quantidadeItens > 0);

    while(naoAchou) {

        performance->comparacoes += 1;

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