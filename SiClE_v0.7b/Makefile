all: SiClE

clean:
	rm Phylogeny.o SiClE.o SiClE

Phylogeny.o: Phylogeny.cpp Phylogeny.h
	g++ -c Phylogeny.cpp

SiClE.o: SiClE.cpp SiClE.h
	g++ -c SiClE.cpp

SiClE: main.cpp SiClE.o Phylogeny.o
	g++ -o SiClE main.cpp SiClE.o Phylogeny.o

