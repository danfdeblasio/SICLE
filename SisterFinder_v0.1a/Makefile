all: SisterFinder

clean:
	rm Phylogeny.o SisterFinder.o SisterFinder

Phylogeny.o: Phylogeny.cpp Phylogeny.h
	g++ -c Phylogeny.cpp

SisterFinder.o: SisterFinder.cpp SisterFinder.h
	g++ -c SisterFinder.cpp

SisterFinder: main.cpp SisterFinder.o Phylogeny.o
	g++ -o SisterFinder main.cpp SisterFinder.o Phylogeny.o

