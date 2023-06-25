#ifndef ARVORE_B2_H
#define ARVORE_B2_H

#define ORDEM 2
#define MM 4

typedef long TipoChave;

typedef struct TipoRegistro{
    TipoChave Chave;
}TipoRegistro;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina{
    short n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
}TipoPagina;



void inicializa(TipoApontador Arvore);

void insereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir);

void insere(TipoRegistro Reg, TipoApontador *Ap);

void ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno);

void Pesquisa(TipoRegistro *x, TipoApontador Ap, int* contador);

void teste();

#endif