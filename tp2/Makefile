all: main.o merge.o desempenho.o aluno.o converteBin.o
	@g++ -o exe main.o merge.o desempenho.o aluno.o converteBin.o -Wall -g
	@rm -r *.o
	
main.o: main.cpp
	@g++ -c main.cpp -Wall -g

merge.o: merge.cpp
	@g++ -c merge.cpp -Wall -g

desempenho.o: desempenho.cpp
	@g++ -c desempenho.cpp -Wall -g

aluno.o: aluno.cpp
	@g++ -c aluno.cpp -Wall -g

converteBin.o: converteBin.cpp
	@g++ -c converteBin.cpp -Wall -g

run:
	@./exe
