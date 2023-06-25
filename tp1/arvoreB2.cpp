#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "arvoreB2.h"

using namespace std;

void insereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir){
    short NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while(NaoAchouPosicao){
        if(Reg.Chave >= Ap->r[k-1].Chave){
            NaoAchouPosicao = false;
            break;
        }
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;
        if(k < 1)
            NaoAchouPosicao = false;
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}




void ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno){
    
    long i = 1;
    long j;
    
    TipoApontador ApTemp;

    if(Ap == NULL){
        *Cresceu = true;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }

    while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave)
        i++;
    
    if(Reg.Chave == Ap->r[i - 1].Chave){
        cout << "Erro: Registro já está presente" << endl;
        *Cresceu = false;
        return; 
    }
    
    if(Reg.Chave < Ap->r[i - 1].Chave)
        i--;
    
    ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
    
    if(!*Cresceu)
        return;
    
    if(Ap->n < MM){
        insereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = false;
        return;
    }

    //Overflow - Tem que dividir
    ApTemp = (TipoApontador) malloc (sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    if(i < ORDEM + 1){
        insereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM]);
        Ap->n--;
        insereNaPagina(Ap, *RegRetorno,*ApRetorno);
    }
    else 
        insereNaPagina(ApTemp, *RegRetorno,*ApRetorno);

    for(j = ORDEM + 2; j <= MM; j++)
        insereNaPagina(ApTemp, Ap->r[j - 1],Ap->p[j]);

    Ap->n = ORDEM; 
    ApTemp->p[0] = Ap->p[ORDEM + 1];

    *RegRetorno = Ap->r[ORDEM];
    *ApRetorno = ApTemp;
}



void insere(TipoRegistro Reg, TipoApontador *Ap){
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoPagina *ApRetorno, *ApTemp;
    ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if(Cresceu){
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap; 
        *Ap = ApTemp;
    }
}






void Pesquisa(TipoRegistro *x, TipoApontador Ap, int *contador){
    long i = 1;
    if(Ap == NULL){
        printf("TipoRegistro não esta na arvore\n");
        return;
    }
    while(i < Ap->n && x->Chave > Ap->r[i-1].Chave) i++;
    if(x->Chave == Ap->r[i-1].Chave){
        (*contador)++;
        *x = Ap->r[i-1];
        return;
    }
    if(x->Chave < Ap->r[i-1].Chave)
    Pesquisa(x, Ap->p[i-1], contador);
    else Pesquisa(x, Ap->p[i], contador);
}

void teste(){

    TipoApontador arvoreB = NULL;
    TipoRegistro registro;
    inicializa(arvoreB);

    for(int i = 0; i < 500; i++){
        registro.Chave = i;
        insere(registro, &arvoreB);
    }

    int registrosEncontrados = 0;

    for(int i = 0; i < 500; i++){
        registro.Chave = i;
        Pesquisa(&registro, arvoreB, &registrosEncontrados);
    }

    cout << "Registros Encontrados: " << registrosEncontrados;

}

void inicializa(TipoApontador Arvore){
    Arvore = NULL;
}
