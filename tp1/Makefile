all: main.o converteBin.o struct.o sequencial.o gerarRegistros.o
	@g++ -o exe main.o converteBin.o struct.o sequencial.o gerarRegistros.o -Wall -g
	@rm -r *.o
main.o: main.cpp
	@g++ -c main.cpp -Wall
converteBin.o: converteBin.cpp
	@g++ -c converteBin.cpp -Wall
struct.o: struct.cpp
	@g++ -c struct.cpp -Wall
#	arquivo.o: arquivo.cpp
#	@g++ -c arquivo.cpp -Wall
sequencial.o: sequencial.cpp
	@g++ -c sequencial.cpp -Wall
gerarRegistros.o: gerarRegistros.cpp
	@g++ -c gerarRegistros.cpp -Wall
run:
	@./exe