all: main.o merge.o aluno.o binarios.o quickSortExterno.o fitas.o intercalacaoBalanceada.o ordenacaoInterna.o selecaoSubstituicao.o heap.o desempenho.o
	@g++ -o ordena main.o merge.o aluno.o binarios.o quickSortExterno.o fitas.o intercalacaoBalanceada.o ordenacaoInterna.o selecaoSubstituicao.o heap.o desempenho.o -Wall -g
	@rm -r *.o
	
main.o: main.cpp
	@g++ -c main.cpp -Wall -g

merge.o: merge.cpp
	@g++ -c merge.cpp -Wall -g

aluno.o: aluno.cpp
	@g++ -c aluno.cpp -Wall -g

binarios.o: binarios.cpp
	@g++ -c binarios.cpp -Wall -g

quickSortExterno.o: quickSortExterno.cpp
	@g++ -c quickSortExterno.cpp -Wall -g

fitas.o: fitas.cpp
	@g++ -c fitas.cpp -Wall -g 

intercalacaoBalanceada.o: intercalacaoBalanceada.cpp
	@g++ -c intercalacaoBalanceada.cpp -Wall -g 

ordenacaoInterna.o: ordenacaoInterna.cpp
	@g++ -c ordenacaoInterna.cpp -Wall -g 

selecaoSubstituicao.o: selecaoSubstituicao.cpp
	@g++ -c selecaoSubstituicao.cpp -Wall -g 

heap.o: heap.cpp
	@g++ -c heap.cpp -Wall -g 

desempenho.o: desempenho.cpp
	@g++ -c desempenho.cpp -Wall -g 

run:
	@./ordena
