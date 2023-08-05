all: main.o merge.o aluno.o converteBin.o quickSortExterno.o fitas.o intercalacaoBalanceada.o ordenacaoInterna.o selecaoSubstituicao.o
	@g++ -o exe main.o merge.o aluno.o converteBin.o quickSortExterno.o fitas.o intercalacaoBalanceada.o ordenacaoInterna.o selecaoSubstituicao.o -Wall -g
	@rm -r *.o
	
main.o: main.cpp
	@g++ -c main.cpp -Wall -g

merge.o: merge.cpp
	@g++ -c merge.cpp -Wall -g

aluno.o: aluno.cpp
	@g++ -c aluno.cpp -Wall -g

converteBin.o: converteBin.cpp
	@g++ -c converteBin.cpp -Wall -g

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

run:
	@./exe
