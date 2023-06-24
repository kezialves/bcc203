all: main.o converteBin.o struct.o sequencial.o gerarRegistros.o arvoreBinaria.o arvoreB.o
	@g++ -o exe main.o converteBin.o struct.o sequencial.o gerarRegistros.o arvoreBinaria.o arvoreB.o -Wall -g
	@rm -r *.o
main.o: main.cpp
	@g++ -c main.cpp -Wall -g
converteBin.o: converteBin.cpp
	@g++ -c converteBin.cpp -Wall -g
struct.o: struct.cpp
	@g++ -c struct.cpp -Wall -g
sequencial.o: sequencial.cpp
	@g++ -c sequencial.cpp -Wall -g
gerarRegistros.o: gerarRegistros.cpp
	@g++ -c gerarRegistros.cpp -Wall -g
arvoreBinaria.o: arvoreBinaria.cpp
	@g++ -c arvoreBinaria.cpp -Wall -g
arvoreB.o: arvoreB.cpp
	@g++ -c arvoreB.cpp -Wall -g
run:
	@./exe