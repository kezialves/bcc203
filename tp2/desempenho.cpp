#include <iostream>

#include "argumentos.h"
#include "desempenho.h"

using namespace std;

void printTabelaLatex(Argumentos *argumentos, Desempenho *desempenhoCriacao, Desempenho *desempenhoIntercalacao) {

    // Obs.: No caso do QuickSort, os dois prints vão ser iguais

    cout << "Criação dos blocos" << endl
        << argumentos->quantidadeAlunos
        << " & " << desempenhoCriacao->tempoExecucao.count()
        << " & " << desempenhoCriacao->comparacoes
        << " & " << desempenhoCriacao->transferenciasLeitura
        << " & " << desempenhoCriacao->transferenciasEscrita
        << " \\\\ \\hline" << endl << endl;
    
    cout << "Intercalação" << endl
        << argumentos->quantidadeAlunos
        << " & " << desempenhoIntercalacao->tempoExecucao.count()
        << " & " << desempenhoIntercalacao->comparacoes
        << " & " << desempenhoIntercalacao->transferenciasLeitura
        << " & " << desempenhoIntercalacao->transferenciasEscrita
        << " \\\\ \\hline" << endl << endl;
}