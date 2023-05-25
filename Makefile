mmap-csv: main.o
	g++ main.o -o mmap-csv

mmap-csv.o: main.cpp
	g++ -c main.cpp -o main.o